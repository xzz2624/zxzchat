//
// Created by zxzzsdb on 2026/8/22.
//

#ifndef GATESERVER_CSERVER_H
#define GATESERVER_CSERVER_H
#include "const.h"

class CServer: public std::enable_shared_from_this<CServer>{
public:
    CServer(boost::asio::io_context &ioc,unsigned short &port);
    void Start();
    ~CServer();
private:
    tcp::acceptor _acceptor;
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::socket _socket;
};


#endif //GATESERVER_CSERVER_H
