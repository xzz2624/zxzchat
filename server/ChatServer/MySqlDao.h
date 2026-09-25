//
// Created by zxzzsdb on 2026/9/1.
//

#ifndef GATESERVER_MYSQLDAO_H
#define GATESERVER_MYSQLDAO_H
#include <iostream>
#include "MySqlConPool.h"
struct UserInfo {
    std::string name;
    std::string pwd;
    int uid;
    std::string email;
};

class MySqlDao
{
public:
    MySqlDao();
    ~MySqlDao();
    int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
//    int RegUserTransaction(const std::string& name, const std::string& email, const std::string& pwd, const std::string& icon);
    bool CheckEmail(const std::string& name, const std::string & email);
    bool UpdatePwd(const std::string& name, const std::string& newpwd);
    bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
//    bool TestProcedure(const std::string& email, int& uid, std::string& name);
private:
    std::unique_ptr<MySqlConPool> _pool;
};


#endif //GATESERVER_MYSQLDAO_H
