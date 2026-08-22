cmake_minimum_required(VERSION 3.31)
project(GateServer)

set(CMAKE_CXX_STANDARD 20)

add_executable(GateServer main.cpp
        CServer.cpp
        CServer.h
        const.h
        HttpConnection.cpp
        HttpConnection.h
        LogicSystem.cpp
        LogicSystem.h
        Singleton.h)

list(APPEND CMAKE_PREFIX_PATH
        "C:/cppsoft/jsoncpp_1_9_8"
)

find_package(jsoncpp CONFIG REQUIRED)

# Boost：你当前使用的是头文件形式
target_include_directories(GateServer PRIVATE
        "C:/cppsoft/boost_1_91_0"
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
        JsonCpp::JsonCpp
)