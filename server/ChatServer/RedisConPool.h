//
// Created by zxzzsdb on 2026/8/31.
//

#ifndef GATESERVER_REDISCONPOOL_H
#define GATESERVER_REDISCONPOOL_H

#include <iostream>
#include <sw/redis++/redis++.h>
#include <atomic>
#include <queue>

class RedisConPool{
public:
    RedisConPool(size_t poolSize ,const char* host,int port,const char* pwd);
    ~RedisConPool();
    redisContext* getConnection();
    void returnConnection(redisContext * context);
    void ClearConnections();
    void Close();
private:
    std::atomic<bool> _b_stop;
    size_t _poolSize;
    const char* _host;
    int _port;
    std::queue<redisContext*> _connections;
    std::condition_variable _conv;
    std::mutex _mutex;
};

#endif //GATESERVER_REDISCONPOOL_H
