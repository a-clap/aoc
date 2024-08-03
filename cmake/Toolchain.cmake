# Dirty hack for clang-tidy
add_compile_definitions(-D__cpp_concepts=202002L)

set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE "POST_BUILD" CACHE STRING "" FORCE)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchains/gcc.cmake")
