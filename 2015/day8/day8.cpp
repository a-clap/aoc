#include <algorithm>
#include <cctype>
#include <cstddef>
#include <ranges>
#include <span>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <bits/ranges_algo.h>

#include "input.hpp"

struct string_mem_t {
    std::size_t size;
    std::size_t characters;
    std::size_t size_encoded;

    constexpr auto operator<=>(string_mem_t const&) const = default;
};

namespace {

string_mem_t parse(std::string_view line) {
    string_mem_t result {
        .size = line.size(),
        .characters = 0,
        .size_encoded = line.size() + 4,
    };

    line.remove_suffix(1);
    line.remove_prefix(1);

    for (auto pos = 0U; pos < line.size();) {
        auto const next_ch = line[pos];
        // do we need to escape?
        if (next_ch == '\\') {
            switch (auto const next = line[pos + 1]; next) {
                case 'x':
                    pos += 3;

                    result.size_encoded += 1;
                    break;
                case '\\':
                case '"':
                    result.size_encoded += 2;
                    pos++;
                    break;
                default:
                    break;
            }
        }

        result.characters += 1;
        pos++;
    }

    return result;
}

std::size_t total_mem(std::span<string_mem_t const> results) {
    return std::ranges::fold_left(results, std::size_t { 0 }, [&](auto sum, auto const& result) {
        return sum + result.size;
    });
}

std::size_t characters(std::span<string_mem_t const> results) {
    return std::ranges::fold_left(results, std::size_t { 0 }, [&](auto sum, auto const& result) {
        return sum + result.characters;
    });
}

std::size_t total_mem_encoded(std::span<string_mem_t const> results) {
    return std::ranges::fold_left(results, std::size_t { 0 }, [&](auto sum, auto const& result) {
        return sum + result.size_encoded;
    });
}

std::size_t calculate_part1(std::vector<string_mem_t> const& results) {
    return total_mem(std::span { results }) - characters(std::span { results });
}

std::size_t calculate_part2(std::vector<string_mem_t> const& results) {
    return total_mem_encoded(std::span { results }) - total_mem(std::span { results });
}
}  // namespace

TEST(day8, parse_line) {
    struct args_t {
        std::string_view input;
        string_mem_t expected;
    };

    static constexpr args_t kArgs[] = {
        {
            .input = R"("")",
            .expected = {
                .size = 2,
                .characters = 0,
                .size_encoded = 6,
            },
        },
        {
            .input = R"("abc")",
            .expected = {
                .size = 5,
                .characters = 3,
                .size_encoded = 9,
            },
        },
        {
            .input = R"("aaa\"aaa")",
            .expected = {
                .size = 10,
                .characters = 7,
                .size_encoded = 16,
            },
        },
        {
            .input = R"("\x27")",
            .expected = {
                .size = 6,
                .characters = 1,
                .size_encoded = 11,
            },
        },
    };
    {
        std::vector<string_mem_t> results;
        for (auto const& [input, expected] : kArgs) {
            SCOPED_TRACE(input);
            auto got = parse(input);
            ASSERT_EQ(expected, got);
            results.push_back(got);
        }
        auto const resp = calculate_part1(results);
        ASSERT_EQ(12, resp);
        auto const resp2 = calculate_part2(results);
        ASSERT_EQ(19, resp2);
    }

    std::vector<string_mem_t> part1;

    for (auto line : kInput | std::views::split('\n')) {
        part1.emplace_back(parse({ line.begin(), line.end() }));
    }

    ASSERT_EQ(1333, calculate_part1(part1));
    ASSERT_EQ(1333, calculate_part2(part1));
}
