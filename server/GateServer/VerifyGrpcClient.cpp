//
// Created by zxzzsdb on 2026/8/25.
//

#include "VerifyGrpcClient.h"

#include <chrono>

GetVerifyRsp VerifyGrpcClient::GetVerifyCode(std::string email) {
    ClientContext context;
    GetVerifyRsp response;
    GetVerifyReq request;
    request.set_email(email);


    auto stub = _pool->getConnection();
    if (!stub) {
        response.set_error(ErrorCodes::RPCFailed);
        return response;
    }

    context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(3));
    Status status = stub->GetVerifyCode(&context, request, &response);

    if (status.ok()) {
        _pool->returnConnection(std::move(stub));
        return response;
    }
    else {
        _pool->returnConnection(std::move(stub));
        response.set_error(ErrorCodes::RPCFailed);
        return response;
    }
}

VerifyGrpcClient::VerifyGrpcClient() {
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string host = gCfgMgr["VerifyServer"]["Host"];
    std::string port = gCfgMgr["VerifyServer"]["Port"];

    _pool.reset(new RPConPool(5, host, port));
}
