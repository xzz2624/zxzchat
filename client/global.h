
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
extern std::function<void(QWidget*)> repolish;

#endif // GLOBAL_H
