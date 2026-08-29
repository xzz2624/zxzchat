//
// Created by zxzzsdb on 2026/8/25.
//验证码服务
//

#ifndef GATESERVER_VERIFYGRPCCLIENT_H
#define GATESERVER_VERIFYGRPCCLIENT_H
#include "grpcpp/grpcpp.h"
#include "message.pb.h"
#include "const.h"
#include "Singleton.h"
#include "message.grpc.pb.h"
#include "ConfigMgr.h"
#include "RPConPool.h"
#
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;



class VerifyGrpcClient :public Singleton<VerifyGrpcClient>{
friend class Singleton<VerifyGrpcClient>;
public:
    GetVerifyRsp GetVerifyCode(std::string email);

private:
    VerifyGrpcClient();
    std::unique_ptr<RPConPool> _pool;
};


#endif //GATESERVER_VERIFYGRPCCLIENT_H
