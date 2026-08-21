
/******************************************************************************
 *
 * @file       global.h
 * @brief      repolish 用来实现页面刷新
 *
 * @author     zxz
 * @date       2026/08/20
 * @history
 *****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QStyle>
#include <QRegularExpression>

#define emit

extern std::function<void(QWidget*)> repolish;

enum ReqId{
    ID_GET_VARIFY_CODE = 1001, //获取验证码
    ID_REG_USER = 1002, //注册用户
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1, //Json解析失败
    ERR_NETWORK = 2,
};

enum Modules{//消息属于哪个模块
    REGISTERMOD = 0,
};

#endif // GLOBAL_H
