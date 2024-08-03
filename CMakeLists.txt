cmake_minimum_required(VERSION 3.21.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile commands" FORCE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

option(WITH_ASAN "Use Address Sanitizer" ON)
option(WITH_UBSAN "Use UB Sanitizer" ON)
option(WITH_LEAKSAN "Use leak Sanitizer" OFF)
option(WITH_TSAN "Use Thread Sanitizer" OFF)
option(WITH_MSAN "Use Memory Sanitizer" OFF)

include(Toolchain)
include(Ccache)

project(aoc
    VERSION
    0.1.0
)

include(GNUInstallDirs)
enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "" FORCE)
endif ()

find_package(GTest REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

add_subdirectory(2015)

include(ReportVariable)
report_variable(CMAKE_VERSION)
report_variable(CMAKE_TOOLCHAIN_FILE)
report_variable(CMAKE_INSTALL_PREFIX)
report_variable(CMAKE_C_COMPILER)
report_variable(CMAKE_C_COMPILER_ARG1)
report_variable(CMAKE_CXX_COMPILER)
report_variable(CMAKE_CXX_COMPILER_ARG1)
report_variable(CMAKE_C_FLAGS)
report_variable(CMAKE_CXX_FLAGS)
report_variable(CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE})
report_variable(CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE})
report_variable(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE)
