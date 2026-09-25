//
// Created by zxzzsdb on 2026/8/31.
//

#include "RedisConPool.h"
RedisConPool::RedisConPool(size_t poolSize, const char *host, int port, const char* pwd) :
_b_stop(false),_host(host),_poolSize(poolSize),_port(port){
    for (int i = 0; i < _poolSize; ++i) {
        auto context = redisConnect(_host,_port);
        if (context == nullptr||context->err != 0){
            if (context != nullptr){
                redisFree(context);
            }
            continue;
        }
        auto reply = (redisReply*)redisCommand(context,"AUTH %s", pwd);
        if (reply->type == REDIS_REPLY_ERROR){
            std::cout << "认证失败" << std::endl;
            //执行失败 释放redisCommand执行后返回的redisReply所占用的内存
            freeReplyObject(reply);
            redisFree(context);
            continue;
        }
        //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
        freeReplyObject(reply);
        std::cout << "认证成功" << std::endl;
        _connections.push(context);
    }
}

RedisConPool::~RedisConPool() {

}

redisContext *RedisConPool::getConnection() {

    std::unique_lock<std::mutex> uniqueLock(_mutex);
    _conv.wait(uniqueLock,[this](){
        if (this->_b_stop) {
            return true;
        }
        return !this->_connections.empty();
    });
    if (_b_stop) {
        return  nullptr;
    }
    auto* context = _connections.front();
    _connections.pop();
    return context;
}

void RedisConPool::returnConnection(redisContext * context) {
    std::unique_lock<std::mutex> uniqueLock(_mutex);
    if (_b_stop){
        redisFree(context);
        return;
    }
    _connections.push(context);
    _conv.notify_one();
}

void RedisConPool::Close() {
    {
        std::lock_guard<std::mutex> lockGuard(_mutex);
        _b_stop = true;
    }
    _conv.notify_all();
}


void RedisConPool::ClearConnections() {
    std::lock_guard<std::mutex> lock(_mutex);
    while (!_connections.empty()) {
        auto* context = _connections.front();
        redisFree(context);
        _connections.pop();
    }
}