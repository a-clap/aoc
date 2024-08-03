#include <format>

#include <gtest/gtest.h>

#include <bits/ranges_algo.h>

#include "input.hpp"

auto count_floors(std::string_view input) {
    static constexpr auto kUp = '(';
    static constexpr auto kDown = ')';

    auto const up = std::ranges::count_if(input, [](auto ch) -> bool {
        return ch == kUp;
    });
    auto const down = std::ranges::count_if(input, [](auto ch) -> bool {
        return ch == kDown;
    });

    return up - down;
}

TEST(day1, input_1) {

    struct args_t {
        std::string_view input;
        int expected;
    };

    args_t args[] = {
        {
            .input = "(())",
            .expected = 0,
        },
        {
            .input = "()()",
            .expected = 0,
        },
        {
            .input = "(((",
            .expected = 3,
        },
        {
            .input = "(()(()(",
            .expected = 3,
        },
        {
            .input = "))(((((",
            .expected = 3,
        },
        {
            .input = "())",
            .expected = -1,
        },
        {
            .input = "))(",
            .expected = -1,
        },
        {
            .input = ")))",
            .expected = -3,
        },
        {
            .input = ")())())",
            .expected = -3,
        },
    };
    for (auto const& arg : args) {
        SCOPED_TRACE(arg.input);

        auto floors = count_floors(arg.input);
        ASSERT_EQ(floors, arg.expected);
    }

    auto const answer = count_floors(kInput);
    ASSERT_EQ(answer, 74);
}
