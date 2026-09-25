//
// Created by zxzzsdb on 2026/9/1.
//

#include "MySqlConPool.h"
#include "const.h"
void MySqlConPool::Close() {
    _b_stop = true;
    _condv.notify_all();
}

MySqlConPool::MySqlConPool(const std::string &url, const std::string &user, const std::string &pass,const std::string &schema, size_t poolSize):
                           _b_stop(false),_url(url),_user(user),_pass(pass),_schema(schema),_poolSize(poolSize){
    try{
        for (int i = 0; i < _poolSize; ++i) {
            sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
            auto*  con = driver->connect(_url, _user, _pass);
            con->setSchema(_schema);
            // 获取当前时间戳
            auto currentTime = std::chrono::system_clock::now().time_since_epoch();
            // 将时间戳转换为秒
            long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
            _pool.push(std::make_unique<SqlConnection>(con, timestamp));
        }

        _check_thread = std::thread([this]{
            while (!_b_stop){
                checkConnection();
                std::this_thread::sleep_for(std::chrono::seconds(60));//心跳机制每60s检查并重连
            }
        });

        _check_thread.detach();
    }catch (sql::SQLException& e){
        // 处理异常
        std::cout << "mysql pool init failed, error is " << e.what()<< std::endl;
    }
}

void MySqlConPool::checkConnection() {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    int poolSize = _pool.size();
    //获取当前时间戳
    auto currentTime = std::chrono::system_clock::now().time_since_epoch();
    //将时间戳转化为秒
    long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
    for (int i = 0; i < _poolSize; i++) {
        auto con = std::move(_pool.front());
        _pool.pop();
        Defer defer([this, &con]() {
            _pool.push(std::move(con));
        });

        if (timestamp - con->_last_oper_time < 5) {
            continue;
        }

        try {
            std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
            stmt->executeQuery("SELECT 1");
            con->_last_oper_time = timestamp;
            //std::cout << "execute timer alive query , cur is " << timestamp << std::endl;
        }
        catch (sql::SQLException& e) {
            std::cout << "Error keeping connection alive: " << e.what() << std::endl;
            // 重新创建连接并替换旧的连接
            sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
            auto* newcon = driver->connect(_url, _user, _pass);
            newcon->setSchema(_schema);
            con->_con.reset(newcon);
            con->_last_oper_time = timestamp;
        }
    }
}

std::unique_ptr<SqlConnection> MySqlConPool::getConnection() {
    std::unique_lock<std::mutex> lock(_mutex);
    _condv.wait(lock, [this] {
        if (_b_stop) {
            return true;
        }
        return !_pool.empty(); });
    if (_b_stop) {
        return nullptr;
    }
    std::unique_ptr<SqlConnection> con(std::move(_pool.front()));
    _pool.pop();
    return con;
}

void MySqlConPool::returnConnection(std::unique_ptr<SqlConnection> connection) {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_b_stop) {
        return;
    }
    _pool.push(std::move(connection));
    _condv.notify_one();
}

MySqlConPool::~MySqlConPool() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_pool.empty()) {
        _pool.pop();
    }
}
