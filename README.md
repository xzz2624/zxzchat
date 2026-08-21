# Qt 聊天客户端开发记录

## Day 1：登录与注册界面

### 已完成

- 搭建 Qt Widgets 客户端基础工程，配置窗体、资源文件、中文翻译、应用图标及全局 QSS 样式加载。
- 完成固定为 `300 × 500` 的登录页和注册页界面：登录页含用户、密码、忘记密码、登录与注册入口；注册页含用户名、邮箱、密码、确认密码、验证码及确认/取消按钮。
- 将登录页与注册页嵌入主窗口，点击“注册”可切换到注册页；两个页面均设置为无边框窗口。
- 注册页密码和确认密码输入框启用密码回显模式。
- 增加注册页提示信息，并通过 QSS 区分正常（绿色）与错误（红色）状态。
- 实现邮箱格式校验：格式不合法时提示“邮箱地址不正确”。

### 函数说明

| 函数 | 功能 |
| --- | --- |
| `main()` | 初始化 Qt 应用；加载系统语言翻译与全局 QSS；创建并显示主窗口。 |
| `repolish(QWidget *w)` | 重新应用控件样式，使动态属性变更后立即刷新 QSS 效果。 |
| `LoginDialog::LoginDialog()` | 初始化登录界面，并将“注册”按钮点击事件关联到页面切换信号。 |
| `MainWindow::MainWindow()` | 创建登录/注册页面，默认显示登录页，并建立登录页到注册页的切换连接。 |
| `MainWindow::SlotSwitchReg()` | 将主窗口中央内容切换为注册页，隐藏登录页并显示注册页。 |
| `RegisterDialog::RegisterDialog()` | 初始化注册界面，设置密码输入显示方式及提示文本的默认样式状态。 |
| `RegisterDialog::on_get_code_clicked()` | 获取邮箱输入并校验格式；非法邮箱显示错误提示；验证码发送接口预留。 |
| `RegisterDialog::showTip(QString str, bool b_ok)` | 设置提示文案和成功/错误状态，并刷新对应的 QSS 样式。 |

### 待完成

- 接入验证码发送、注册提交与服务端通信。
- 实现登录校验、忘记密码、注册页返回/取消等交互。

## Day 2：注册模块网络通信基础

### 已完成

- 在 Qt 工程中启用 `network` 模块，并新增 `HttpMgr` 网络管理器与通用单例模板，为客户端网络请求提供统一入口。
- 实现 JSON 格式 HTTP POST 请求：自动设置 `application/json` 请求头和内容长度，异步读取响应，并区分网络成功与网络错误。
- 增加请求 ID、错误码和模块枚举，支持按业务模块分发网络请求结果；当前已接入注册模块的完成信号。
- 注册页建立网络响应连接，支持接收注册模块请求结果，并对网络错误、JSON 解析失败和非对象 JSON 响应给出界面提示。
- 为注册模块建立按请求 ID 分发的回调处理表；已实现“获取验证码”响应处理：服务端返回成功时提示验证码已发送，失败时提示参数错误。

### 函数说明

| 函数 | 功能 |
| --- | --- |
| `Singleton<T>::GetInstance()` | 线程安全地创建并获取指定类型的全局单例实例。 |
| `HttpMgr::HttpMgr()` | 初始化网络管理器，并连接通用网络完成信号与模块分发槽。 |
| `HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)` | 发送 JSON HTTP POST 请求；异步处理响应并返回成功或网络错误结果。 |
| `HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)` | 按模块分发网络请求结果；当前将注册模块结果转发给注册页。 |
| `RegisterDialog::initHttpHandlers()` | 初始化注册模块的请求回调表，登记获取验证码请求的响应处理逻辑。 |
| `RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)` | 校验网络状态并解析 JSON 响应，再按请求 ID 调用对应业务回调。 |

### 待完成

- 在“获取验证码”按钮点击事件中实际调用 `HttpMgr::PostHttpReq()`，并配置验证码服务端接口。
- 补充注册提交请求及其成功、失败、字段校验等响应处理。
- 完善未注册请求 ID 的兜底处理，并接入登录、忘记密码等模块的网络通信。
