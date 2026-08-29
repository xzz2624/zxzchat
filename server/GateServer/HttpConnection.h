//
// Created by zxzzsdb on 2026/8/22.
//

#ifndef GATESERVER_HTTPCONNECTION_H
#define GATESERVER_HTTPCONNECTION_H
#include "const.h"

class HttpConnection :public std::enable_shared_from_this<HttpConnection>{
    friend class LogicSystem;
public:
    HttpConnection(boost::asio::io_context& ioc);
    void Start();
    tcp::socket& GetSocket() {
        return _socket;
    }
private:
    void CheckDeadline();//超时检测
    void WriteResponse();
    void PreParseGetParam();
    void HandleReq();

    tcp::socket  _socket;
    // The buffer for performing reads.
    beast::flat_buffer  _buffer{ 8192 };//8KB

    // The request message.
    http::request<http::dynamic_body> _request;

    // The response message.
    http::response<http::dynamic_body> _response;

    // The timer for putting a deadline on connection processing.
    net::steady_timer _deadline{
            _socket.get_executor(), std::chrono::seconds(60) };

    //解析GET请求url参数
    std::string _get_url;
    std::unordered_map<std::string, std::string> _get_params;

};


#endif //GATESERVER_HTTPCONNECTION_H
