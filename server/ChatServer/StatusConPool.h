//
// Created by zxzzsdb on 2026/9/9.
//

#ifndef GATESERVER_STATUSCONPOOL_H
#define GATESERVER_STATUSCONPOOL_H
#include "const.h"
#include "ConfigMgr.h"
#include <grpcpp/grpcpp.h>
#include "message.pb.h"
#include "message.grpc.pb.h"
#include <atomic>
#include <iostream>
#include <queue>
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
class StatusConPool {
public:
    StatusConPool(size_t poolSize, std::string host, std::string port);
    ~StatusConPool();

    std::unique_ptr<StatusService::Stub> getConnection();

    void returnConnection(std::unique_ptr<StatusService::Stub> context);
    void Close();
private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<StatusService::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};


#endif //GATESERVER_STATUSCONPOOL_H
