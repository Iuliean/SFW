enable_testing()

set(SFW_UNIT_TEST "${PROJECT_NAME}_unit_test")

add_executable(${SFW_UNIT_TEST}
    test_main.cpp
)

target_compile_definitions(${SFW_UNIT_TEST} PUBLIC ${SFW_PREPROCESSOR_DEFINES})

target_link_libraries(${SFW_UNIT_TEST} GTest::gtest_main)

target_include_directories(${SFW_UNIT_TEST}
    PRIVATE ${PROJECT_SOURCE_DIR}/include/SFW)