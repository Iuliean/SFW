
set(SFW_SRC_FILES "Connection.cpp"
                  "LoggerManager.cpp"
                  "Server.cpp"
                  "ServerConnectionHandler.cpp"
                  "ThreadPool.cpp"
                  "utils.cpp"
)

if (WIN32)
    set(SFW_SRC_FILES "${SFW_SRC_FILES}"
                      "platform/win32/Socket.cpp"
                      "platform/win32/SocketDescriptor.cpp"
                      "platform/win32/WinSockInstance.cpp")
else()
    set(SFW_SRC_FILES "${SFW_SRC_FILES}"
                      "platform/posix/Socket.cpp"
                      "platform/posix/SocketDescriptor.cpp")
    set(LINK_LIBS "pthread")
endif()


add_library(${PROJECT_NAME} SHARED  ${SFW_SRC_FILES})

#includes main.cpp for when i want to test stuff 
add_executable(${PROJECT_NAME}-test
    main.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ${SFW_PREPROCESSOR_DEFINES})
target_compile_definitions(${PROJECT_NAME}-test PUBLIC ${SFW_PREPROCESSOR_DEFINES})

target_include_directories(${PROJECT_NAME}
    PRIVATE     ${PROJECT_SOURCE_DIR}/include/SFW)

target_include_directories(${PROJECT_NAME}-test
    PRIVATE ${PROJECT_SOURCE_DIR}/include/SFW)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE)

    target_link_libraries(${PROJECT_NAME}-test
        PRIVATE ${PROJECT_NAME})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBS})
    
    target_link_libraries(${PROJECT_NAME}-test
        PRIVATE ${PROJECT_NAME} ${LINK_LIBS})
endif()