//
// Created by zxzzsdb on 2026/9/2.
//

#ifndef GATESERVER_MYSQLMGR_H
#define GATESERVER_MYSQLMGR_H
#include "Singleton.h"
#include "MySqlDao.h"

class MySqlMgr: public Singleton<MySqlMgr>
{
    friend class Singleton<MySqlMgr>;
public:
    ~MySqlMgr();
    int RegUser(const std::string& name, const std::string& email,  const std::string& pwd);
    bool CheckEmail(const std::string& name, const std::string & email);
    bool UpdatePwd(const std::string& email, const std::string& pwd);
    bool CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo);
//    bool TestProcedure(const std::string &email, int& uid, std::string & name);
private:
    MySqlMgr();
    MySqlDao _dao;
};


#endif //GATESERVER_MYSQLMGR_H
