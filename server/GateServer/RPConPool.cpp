//
// Created by zxzzsdb on 2026/8/29.
//

#include "RPConPool.h"

#include <utility>
RPConPool::RPConPool(size_t poolSize, std::string host, std::string port)
    : _b_stop(false),
      _poolSize(poolSize),
      _host(std::move(host)),
      _port(std::move(port)) {
    for (size_t i = 0; i < _poolSize; ++i) {

        std::shared_ptr<Channel> channel = grpc::CreateChannel(_host + ":" + _port,
                                                               grpc::InsecureChannelCredentials());

        _connections.push(VerifyService::NewStub(channel));
    }
}

RPConPool::~RPConPool() {
    std::lock_guard<std::mutex> lock(_mutex);
    Close();
    while (!_connections.empty()) {
        _connections.pop();
    }
}

std::unique_ptr<VerifyService::Stub> RPConPool::getConnection() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [this] {
        if (_b_stop) {
            return true;
        }
        return !_connections.empty();
    });
    //如果停止则直接返回空指针
    if (_b_stop) {
        return  nullptr;
    }
    auto context = std::move(_connections.front());
    _connections.pop();
    return context;
}

void RPConPool::returnConnection(std::unique_ptr<VerifyService::Stub> context) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_b_stop) {
        return;
    }
    _connections.push(std::move(context));
    _cond.notify_one();
}

void RPConPool::Close() {
    _b_stop = true;
    _cond.notify_all();
}
