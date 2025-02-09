
set(SFW_SRC_FILES "Connection.cpp"
                  "LoggerManager.cpp"
                  "Server.cpp"
                  "ServerConnectionHandler.cpp"
                  "ThreadPool.cpp"
                  "utils.cpp"
)

set(SFW_SHARED_LINKS "")

if (WIN32)
    set(SFW_SRC_FILES "${SFW_SRC_FILES}"
                      "platform/win32/Socket.cpp"
                      "platform/win32/SocketDescriptor.cpp"
                      "platform/win32/WinSockInstance.cpp")
    set(LINK_LIBS "${SFW_SHARED_LINKS}"
                  "ws2_32")
else()
    set(SFW_SRC_FILES "${SFW_SRC_FILES}"
                      "platform/posix/Socket.cpp"
                      "platform/posix/SocketDescriptor.cpp")
    set(LINK_LIBS "${SFW_SHARED_LINKS}"
                  "pthread")
endif()


add_library(${PROJECT_NAME} SHARED  ${SFW_SRC_FILES})



target_compile_definitions(${PROJECT_NAME} PUBLIC ${SFW_PREPROCESSOR_DEFINES} PRIVATE SFW_EXPORT)

target_include_directories(${PROJECT_NAME}
    PRIVATE     ${PROJECT_SOURCE_DIR}/include/SFW)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBS})

install(TARGETS ${PROJECT_NAME})