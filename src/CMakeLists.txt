add_library(${PROJECT_NAME} SHARED
    Connection.cpp
    Logger.cpp
    LoggerManager.cpp
    Server.cpp
    ServerConnectionHandler.cpp
    Socket.cpp
    SocketDescriptor.cpp
    ThreadPool.cpp
    utils.cpp)

#includes main.cpp for when i want to test stuff 
add_executable(${PROJECT_NAME}-test
    main.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_USE_STD_FORMAT)
target_compile_definitions(${PROJECT_NAME}-test PUBLIC SPDLOG_USE_STD_FORMAT)

target_include_directories(${PROJECT_NAME}
    PRIVATE     ${PROJECT_SOURCE_DIR}/include/SFW)

target_include_directories(${PROJECT_NAME}-test
    PRIVATE ${PROJECT_SOURCE_DIR}/include/SFW)

target_link_libraries(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}-test
    PRIVATE ${PROJECT_NAME})