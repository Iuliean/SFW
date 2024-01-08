cmake_minimum_required(VERSION 3.27.7)

project(SFW)

include_directories(include/)
include_directories(dependencies/spdlog/include)


add_subdirectory(dependencies/spdlog)

add_library(${PROJECT_NAME} SHARED
    src/Connection.cpp
    src/Logger.cpp
    src/LoggerManager.cpp
    src/Server.cpp
    src/ServerConnectionHandler.cpp
    src/Socket.cpp
    src/SocketDescriptor.cpp
    src/ThreadPool.cpp
    src/utils.cpp
    )

add_executable(${PROJECT_NAME}-test
    src/main.cpp
    src/Connection.cpp
    src/Logger.cpp
    src/LoggerManager.cpp
    src/Server.cpp
    src/ServerConnectionHandler.cpp
    src/Socket.cpp
    src/SocketDescriptor.cpp
    src/ThreadPool.cpp
    src/utils.cpp
    )

target_link_libraries(SFW PRIVATE spdlog::spdlog_header_only)
target_link_libraries(SFW-test PRIVATE spdlog::spdlog_header_only)

set_property(TARGET SFW PROPERTY CXX_STANDARD 20)
set_property(TARGET SFW-test PROPERTY CXX_STANDARD 20)