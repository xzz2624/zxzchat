cmake_minimum_required(VERSION 3.31)
project(StatusServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Ninja 的 Debug / Release Profile 分别使用对应的 MSVC 第三方库。
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(THIRD_PARTY_CONFIG "debug")
else()
    set(THIRD_PARTY_CONFIG "release")
endif()

# 当前只构建了 MySQL Connector/C++ 的 Debug 版本。旧 JDBC API 使用
# mysqlcppconn.lib / mysqlcppconn-9-vs14.dll，而非 mysqlcppconn8。
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR
            "MySQL Connector/C++ 目前只有 Debug 版本。请使用 Debug Profile，"
            "或先构建 C:/cppsoft/mysql-connector-cpp-msvc-release。")
endif()

set(MYSQL_CONCPP_ROOT "C:/cppsoft/mysql-connector-cpp-msvc-debug")
set(MYSQL_CONCPP_LIB "${MYSQL_CONCPP_ROOT}/lib/vs14/debug/mysqlcppconn.lib")
set(MYSQL_CONCPP_DLL "${MYSQL_CONCPP_ROOT}/lib/debug/mysqlcppconn-9-vs14.dll")
set(MYSQL_OPENSSL_ROOT "C:/cppsoft/openssl-msvc-release/lib")
set(MYSQL_OPENSSL_SSL_DLL "${MYSQL_OPENSSL_ROOT}/libssl-3-x64.dll")
set(MYSQL_OPENSSL_CRYPTO_DLL "${MYSQL_OPENSSL_ROOT}/libcrypto-3-x64.dll")

foreach(required_file IN ITEMS
        "${MYSQL_CONCPP_ROOT}/include/jdbc/cppconn/driver.h"
        "${MYSQL_CONCPP_LIB}"
        "${MYSQL_CONCPP_DLL}"
        "${MYSQL_OPENSSL_SSL_DLL}"
        "${MYSQL_OPENSSL_CRYPTO_DLL}")
    if(NOT EXISTS "${required_file}")
        message(FATAL_ERROR "缺少 MySQL Connector/C++ 依赖文件: ${required_file}")
    endif()
endforeach()

add_executable(StatusServer
        main.cpp
        const.h
        ConfigMgr.cpp
        ConfigMgr.h
        AsioIOServicePool.cpp
        AsioIOServicePool.h
        RedisConPool.cpp
        RedisConPool.h
        RedisMgr.cpp
        RedisMgr.h
        MySqlConPool.cpp
        MySqlConPool.h
        MySqlDao.cpp
        MySqlDao.h
        MySqlMgr.cpp
        MySqlMgr.h
        Singleton.h
        message.grpc.pb.h
        message.grpc.pb.cc
        message.pb.cc
        message.pb.h
)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
        "${CMAKE_CURRENT_BINARY_DIR}/config.ini"
        COPYONLY
)

if(MSVC)
    # 最低支持 Windows 10；若需要兼容 Windows 7，改为 0x0601。
    target_compile_definitions(StatusServer PRIVATE
            WINVER=0x0A00
            _WIN32_WINNT=0x0A00
    )

    # 让 MSVC 按 UTF-8 读取源码，并以 UTF-8 处理字符串字面量。
    target_compile_options(StatusServer PRIVATE
            /utf-8
    )
endif()

list(PREPEND CMAKE_PREFIX_PATH
        "C:/cppsoft/boost-msvc-1_91_0"
        "C:/cppsoft/grpc-msvc-${THIRD_PARTY_CONFIG}"
        "C:/cppsoft/jsoncpp-msvc-${THIRD_PARTY_CONFIG}"
)

# 此 gRPC 安装包的导出目标依赖 Protobuf，但 gRPCConfig.cmake 不会自动加载它。
find_package(Boost 1.91 CONFIG REQUIRED COMPONENTS filesystem)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)

target_include_directories(StatusServer PRIVATE
        "C:/cppsoft/boost-msvc-1_91_0/include"
        "${MYSQL_CONCPP_ROOT}/include"
        "${MYSQL_CONCPP_ROOT}/include/jdbc"
)

target_compile_definitions(StatusServer PRIVATE
        BOOST_ERROR_CODE_HEADER_ONLY
)

target_link_libraries(StatusServer PRIVATE
        ws2_32
        mswsock
        Boost::filesystem
        gRPC::grpc++
        JsonCpp::JsonCpp
        redis++::redis++
        "${MYSQL_CONCPP_LIB}"
)

if(WIN32)
    add_custom_command(TARGET StatusServer POST_BUILD
            # Connector 的运行时 DLL 及其 OpenSSL 依赖放在 exe 同目录，供 Windows 加载。
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MYSQL_CONCPP_DLL}"
            "${MYSQL_OPENSSL_SSL_DLL}"
            "${MYSQL_OPENSSL_CRYPTO_DLL}"
            $<TARGET_FILE_DIR:StatusServer>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:StatusServer>
            $<TARGET_FILE_DIR:StatusServer>
            COMMAND_EXPAND_LISTS
    )
endif()
