//
// Created by zxzzsdb on 2026/9/1.
//

#ifndef GATESERVER_MYSQLCONPOOL_H
#define GATESERVER_MYSQLCONPOOL_H
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <queue>

class SqlConnection {
public:
    SqlConnection(sql::Connection* con, int64_t lasttime):_con(con), _last_oper_time(lasttime){}
    std::unique_ptr<sql::Connection> _con;
    int64_t _last_oper_time;
};

class MySqlConPool {
public:
    MySqlConPool(const std::string& url,const std::string& user,const std::string& pass,const std::string& schema,size_t poolSize);
    ~MySqlConPool();
//    bool reconnect();
//    void checkConnectionPro();
    void checkConnection();//心跳机制防止断连mysql
    std::unique_ptr<SqlConnection> getConnection();
    void returnConnection(std::unique_ptr<SqlConnection> connection);
    void Close();
private:
    std::string _url;
    std::string _user;
    std::string _pass;
    std::string _schema;//使用的数据库
    size_t _poolSize;
    std::mutex _mutex;
    std::condition_variable _condv;
    std::queue<std::unique_ptr<SqlConnection>> _pool;
    bool _b_stop;
    std::thread _check_thread;
};


#endif //GATESERVER_MYSQLCONPOOL_H
