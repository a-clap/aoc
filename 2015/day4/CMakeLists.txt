add_executable(day4 day4.cpp)

target_link_libraries(day4
    PUBLIC
    GTest::gtest_main
    crypto
    fmt::fmt
)
