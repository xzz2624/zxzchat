//
// Created by zxzzsdb on 2026/8/28.
//

#ifndef GATESERVER_ASIOIOSERVICEPOOL_H
#define GATESERVER_ASIOIOSERVICEPOOL_H
#include "const.h"
#include "Singleton.h"

#include <cstddef>
#include <thread>
#include <vector>

class AsioIOServicePool: public Singleton<AsioIOServicePool>{
friend Singleton<AsioIOServicePool>;
public:
    using IOService = boost::asio::io_context;
    using WorkGuard = boost::asio::executor_work_guard<IOService::executor_type>;
    using WorkPtr = std::unique_ptr<WorkGuard>;
    ~AsioIOServicePool();
    AsioIOServicePool(const AsioIOServicePool&) = delete;
    AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
    // 使用 round-robin 的方式返回一个 io_service
    boost::asio::io_context& GetIOService();
    void Stop();
private:
    AsioIOServicePool(std::size_t size = 2/*std::thread::hardware_concurrency()*/);
    std::vector<IOService> _ioServices;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _nextIOService;
};


#endif //GATESERVER_ASIOIOSERVICEPOOL_H
