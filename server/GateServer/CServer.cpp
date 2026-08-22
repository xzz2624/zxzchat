//
// Created by zxzzsdb on 2026/8/22.
//

#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context &ioc, unsigned short &port):
_ioc(ioc),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port)),
_socket(ioc){

}

void CServer::Start() {
    auto self = shared_from_this();
    _acceptor.async_accept(_socket,[self](beast::error_code ec){
        try{
            if(ec){
                //出错则放弃这个链接,监听新链接
                self->Start();
                return ;
            }
            //处理新链接，创建HpptConnection类管理新连接
            std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
            //继续监听
            self->Start();
        }catch(std::exception& e){
            std::cout << "exception is " << e.what() << std::endl;
            self->Start();
        }
    });
}

CServer::~CServer() {

}
