//
// Created by zxzzsdb on 2026/8/29.
//

#ifndef GATESERVER_RPCONPOOL_H
#define GATESERVER_RPCONPOOL_H
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <grpcpp/grpcpp.h>
#include <atomic>
#include <queue>
#include "message.grpc.pb.h"
#include "message.pb.h"
using message::VerifyService;
using grpc::Channel;
class RPConPool {
public:
    RPConPool(size_t poolSize, std::string host, std::string port);

    ~RPConPool();

    std::unique_ptr<VerifyService::Stub> getConnection();

    void returnConnection(std::unique_ptr<VerifyService::Stub> context);

    void Close();

private:
    std::atomic<bool> _b_stop;
    size_t _poolSize;
    std::string _host;
    std::string _port;
    std::queue<std::unique_ptr<VerifyService::Stub>> _connections;
    std::mutex _mutex;
    std::condition_variable _cond;
};


#endif //GATESERVER_RPCONPOOL_H
