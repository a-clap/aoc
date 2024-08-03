#include <algorithm>
#include <array>
#include <charconv>
#include <cstddef>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <system_error>

#include <gtest/gtest.h>

#include <bits/ranges_algo.h>

#include "input.hpp"

struct dimensions_t {
    std::array<int, 3> value;

    auto operator<=>(dimensions_t const&) const = default;
};

static dimensions_t parse_line(std::string_view line) {
    dimensions_t dimensions {};
    for (std::size_t pos = 0; auto view : line | std::views::split('x')) {
        auto str = std::string_view(view.begin(), view.end());
        if (auto [ptr, errc] = std::from_chars(str.data(), str.data() + str.size(), dimensions.value[pos++]); errc != std::errc()) {
            throw std::runtime_error("Invalid input");
        }
    }
    return dimensions;
}

static int wrapping_paper_1(dimensions_t const dimensions) {

    auto const& [length, width, height] = dimensions.value;

    auto const length_width = length * width;
    auto const width_height = width * height;
    auto const height_length = height * length;

    auto const min = std::min({ length_width, width_height, height_length });

    return (2 * (length_width + width_height + height_length)) + min;
}

static int wrapping_paper_2(dimensions_t dimensions) {
    auto const& [length, width, height] = dimensions.value;
    auto const volume = length * width * height;
    std::ranges::sort(dimensions.value);

    return volume + (dimensions.value[0] * 2) + (dimensions.value[1] * 2);
}

TEST(day2, input_1) {
    struct args_t {
        std::string_view name;
        dimensions_t dimensions;

        int expected_first;
        int expected_second;
    };

    static constexpr args_t kArgs[] = {
        {
            .name = "2x3x4",
            .dimensions = { 2, 3, 4 },
            .expected_first = 58,
            .expected_second = 34,
        },
        {
            .name = "1x1x10",
            .dimensions = { 1, 1, 10 },
            .expected_first = 43,
            .expected_second = 14,
        },
    };
    for (auto const& [name, dimensions, expected, second] : kArgs) {
        SCOPED_TRACE(name);

        auto got = parse_line(name);
        ASSERT_EQ(got, dimensions);
        ASSERT_EQ(expected, wrapping_paper_1(got));
        ASSERT_EQ(second, wrapping_paper_2(got));
    }

    constexpr auto kSplitByNl = std::views::split('\n');
    auto input = kInput | kSplitByNl;

    auto const answer_1 = std::ranges::fold_left(input.begin(), input.end(), 0, [](auto const sum, auto const line) {
        auto const dim = parse_line(std::string_view(line));
        return sum + wrapping_paper_1(dim);
    });

    ASSERT_EQ(answer_1, 1598415);

    auto const answer_2 = std::ranges::fold_left(input.begin(), input.end(), 0, [](auto const sum, auto const line) {
        auto const dim = parse_line(std::string_view(line));
        return sum + wrapping_paper_2(dim);
    });

    ASSERT_EQ(answer_2, 3812909);
}
