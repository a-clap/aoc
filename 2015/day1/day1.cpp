#include <string_view>

#include <gtest/gtest.h>

#include <bits/ranges_algo.h>

#include "input.hpp"

static constexpr auto kUp = '(';
[[maybe_unused]] static constexpr auto kDown = ')';

static auto count_floors(std::string_view input) {

    return std::ranges::fold_left(input.begin(), input.end(), 0, [](auto sum, auto sign) {
        if (sign == kUp) {
            return ++sum;
        }
        return --sum;
    });
}

static auto count_floors_2(std::string_view input) {
    static constexpr auto kDestination = -1;
    auto start_floor = 0;

    auto pos = 0;
    for (auto const& character : input) {
        ++pos;
        if (character == kUp) {
            ++start_floor;
            continue;
        }

        if (--start_floor == kDestination) {
            return pos;
        }
    }
    return -1;
}

TEST(day1, input_1) {

    struct args_t {
        std::string_view input;
        int expected;
    };

    static constexpr args_t kArgs[] = {
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
    for (auto const& [input, expected] : kArgs) {
        SCOPED_TRACE(input);

        auto floors = count_floors(input);
        ASSERT_EQ(floors, expected);
    }

    auto const answer = count_floors(kInput);
    ASSERT_EQ(answer, 74);

    auto const answer2 = count_floors_2(kInput);
    ASSERT_EQ(answer2, 1795);
}
