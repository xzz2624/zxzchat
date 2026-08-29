cmake_minimum_required(VERSION 3.31)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Ninja 的 Debug / Release Profile 分别使用对应的 MSVC 第三方库。
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(THIRD_PARTY_CONFIG "debug")
else()
    set(THIRD_PARTY_CONFIG "release")
endif()

add_executable(GateServer main.cpp
        CServer.cpp
        CServer.h
        const.h
        HttpConnection.cpp
        HttpConnection.h
        LogicSystem.cpp
        LogicSystem.h
        Singleton.h
        VerifyGrpcClient.cpp
        VerifyGrpcClient.h
        ConfigMgr.cpp
        ConfigMgr.h
        message.grpc.pb.h
        message.grpc.pb.cc
        message.pb.cc
        message.pb.h
        AsioIOServicePool.cpp
        AsioIOServicePool.h
        RPConPool.cpp
        RPConPool.h)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
        "${CMAKE_CURRENT_BINARY_DIR}/config.ini"
        COPYONLY
)

if(MSVC)
    # 最低支持 Windows 10；若需要兼容 Windows 7，改为 0x0601。
    target_compile_definitions(GateServer PRIVATE
            WINVER=0x0A00
            _WIN32_WINNT=0x0A00
    )

    # 让 MSVC 按 UTF-8 读取源码，并以 UTF-8 处理字符串字面量。
    target_compile_options(GateServer PRIVATE
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

# Boost：你当前使用的是头文件形式
target_include_directories(GateServer PRIVATE
        "C:/cppsoft/boost-msvc-1_91_0/include"
)

target_compile_definitions(GateServer PRIVATE
        BOOST_ERROR_CODE_HEADER_ONLY
)

target_include_directories(GateServer PRIVATE
        "${GENERATED_DIR}"
)

target_link_libraries(GateServer PRIVATE
        ws2_32
        mswsock
        Boost::filesystem
        gRPC::grpc++
        JsonCpp::JsonCpp
)
