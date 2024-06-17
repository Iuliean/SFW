cmake_minimum_required(VERSION 3.27.7)

project(SFW VERSION 0.1)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SPDLOG_USE_STD_FORMAT ON)

include_directories(dependencies/spdlog/include)
add_subdirectory(src)

add_library(SFW_EXPORT INTERFACE)
add_library(SFW::SFW ALIAS SFW_EXPORT)

target_include_directories(SFW_EXPORT
    INTERFACE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(SFW_EXPORT
    INTERFACE SFW)

target_compile_features(SFW_EXPORT
    INTERFACE cxx_std_20)

target_compile_definitions(SFW_EXPORT
    INTERFACE SPDLOG_USE_STD_FORMAT=1)