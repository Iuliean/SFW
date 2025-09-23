cmake_minimum_required(VERSION 3.18.4)

project(SFW VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SPDLOG_USE_STD_FORMAT ON)
option(SHARED_LIB OFF)

include(ExternalProject)
include(FetchContent)
add_subdirectory(dependencies/spdlog)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

if (WIN32)
    set (SFW_PREPROCESSOR_DEFINES "-DNOGDI" "SPDLOG_USE_STD_FORMAT")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
else()
endif()

add_subdirectory(src)
add_subdirectory(tests)

add_library(SFW_EXPORT INTERFACE)
add_library(SFW::SFW ALIAS SFW_EXPORT)

target_include_directories(SFW_EXPORT
    INTERFACE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(SFW_EXPORT
    INTERFACE SFW
    INTERFACE spdlog::spdlog)

target_compile_features(SFW_EXPORT
    INTERFACE cxx_std_20)

target_compile_definitions(SFW_EXPORT
    INTERFACE SPDLOG_USE_STD_FORMAT=1 SFW_IMPORT)