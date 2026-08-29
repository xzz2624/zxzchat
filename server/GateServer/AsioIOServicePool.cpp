//
// Created by zxzzsdb on 2026/8/28.
//

#include "AsioIOServicePool.h"

#include <iostream>

using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size)
    : _ioServices(size), _nextIOService(0) {
    _works.reserve(_ioServices.size());
    for (auto& ioService : _ioServices) {
        _works.emplace_back(
                std::make_unique<WorkGuard>(ioService.get_executor())
        );
    }

    //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
    for (std::size_t i = 0; i < _ioServices.size(); ++i) {
        _threads.emplace_back([this, i]() {
            _ioServices[i].run();
        });
    }
}

AsioIOServicePool::~AsioIOServicePool() {
    Stop();
    std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
    auto& service = _ioServices[_nextIOService++];
    if (_nextIOService == _ioServices.size()) {
        _nextIOService = 0;
    }
    return service;
}

void AsioIOServicePool::Stop(){
    // 移除保活工作；stop() 使仍在等待的 run() 立即返回。
    for (auto& work : _works) {
        work.reset();
    }

    for (auto& ioService : _ioServices) {
        ioService.stop();
    }

    for (auto& t : _threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
