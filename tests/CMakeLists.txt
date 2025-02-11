enable_testing()

find_package(Python COMPONENTS Development)

set(SFW_UNIT_TEST "${PROJECT_NAME}_unit_test")
set(SFW_TEST_SCRIPT_FILES "scripts/TSocket.py")

if(_WIN32)
else()
    set(SFW_TEST_LINK_LIBS "expat"
                           "dl"
                           "util"
                           "z"
                        )
endif()

add_executable(${SFW_UNIT_TEST}
    test_main.cpp
)

target_compile_definitions(${SFW_UNIT_TEST} PUBLIC ${SFW_PREPROCESSOR_DEFINES})

target_link_libraries(${SFW_UNIT_TEST}  ${SFW_TEST_LINK_LIBS} SFW::SFW GTest::gtest_main Python::Python)

target_include_directories(${SFW_UNIT_TEST}
    PRIVATE ${PROJECT_SOURCE_DIR}/include/SFW
    PRIVATE ${PROJECT_SOURCE_DIR}/tests)

install(TARGETS ${SFW_UNIT_TEST} SFW DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
install(FILES ${SFW_TEST_SCRIPT_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)

