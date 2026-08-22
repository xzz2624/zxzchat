//
// Created by zxzzsdb on 2026/8/22.
//

#ifndef GATESERVER_CONST_H
#define GATESERVER_CONST_H

#include "boost/beast.hpp"
#include "boost/beast/http.hpp"
#include "boost/asio.hpp"
#include <iostream>
#include <memory>
#include <functional>
#include <map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
enum ErrorCodes {
    Success = 0,
    Error_Json = 1001,  //Json解析错误
    RPCFailed = 1002,  //RPC请求错误
};

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
#endif //GATESERVER_CONST_H
