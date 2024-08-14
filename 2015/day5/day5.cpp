#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <bits/ranges_algo.h>

#include "input.hpp"

static constexpr bool is_nice(std::string_view const input) {
    static constexpr std::string_view kVowels = "aeiou";
    static constexpr std::array kNaughty = { "ab", "cd", "pq", "xy" };

    if (auto const is_naughty = std::ranges::any_of(kNaughty, [&](auto const& naughty) -> bool {
            return input.contains(naughty);
        });
        is_naughty) {
        return false;
    }

    auto vowels_count = 0;
    auto last_ch = 0xff;
    bool got_double = false;
    for (auto const& inp : input) {
        if (vowels_count < 3) {
            if (kVowels.contains(inp)) {
                vowels_count++;
            }
        }

        if (inp == last_ch) {
            got_double = true;
        }

        last_ch = static_cast<unsigned char>(inp);
        if (got_double && vowels_count >= 3) {
            return true;
        }
    }

    return false;
}

TEST(day5, part1) {
    struct args_t {
        std::string_view input;
        bool is_nice;
    };

    static constexpr args_t kArgs[] = {
        {
            .input = "ugknbfddgicrmopn",
            .is_nice = true,
        },
        {
            .input = "aaa",
            .is_nice = true,
        },
        {
            .input = "jchzalrnumimnmhp",
            .is_nice = false,
        },
        {
            .input = "haegwjzuvuyypxyu",
            .is_nice = false,
        },
        {
            .input = "dvszwmarrgswjxmb",
            .is_nice = false,
        },
    };
    for (auto const& arg : kArgs) {
        SCOPED_TRACE(arg.input);

        ASSERT_EQ(arg.is_nice, is_nice(arg.input));
    }

    auto splitted = kInput | std::views::split('\n');

    auto count = std::ranges::fold_left(splitted, 0, [](auto sum, auto input) {
        if (is_nice({ input.begin(), input.end() })) {
            ++sum;
        }
        return sum;
    });

    ASSERT_EQ(255, count);
}

static constexpr bool is_nice_2(std::string_view const input) {

    std::vector<char> chars;
    std::vector<std::vector<std::size_t>> chars_pos;

    auto got_double_pair = false;
    for (auto i = 0ZU; i < input.size(); i += 1) {
        if (!got_double_pair) {
            auto pair = input.substr(i, 2);
            got_double_pair = input.find(pair, i + 2) != std::string_view::npos;
        }

        auto const iter = std::ranges::find(chars, input[i]);
        if (iter == std::end(chars)) {
            chars.push_back(input[i]);
            chars_pos.emplace_back(std::vector<std::size_t> { i });
        } else {
            auto const pos = static_cast<std::size_t>(std::distance(chars.begin(), iter));
            chars_pos[pos].push_back(i);
        }
    }

    if (!got_double_pair) {
        return false;
    }

    for (auto const& poses : chars_pos) {
        for (auto pos = 0U; pos < (poses.size() - 1); ++pos) {
            auto const next = poses[pos + 1];
            auto const current = poses[pos];
            auto const diff = next - current;
            if (diff == 0 || diff > 2) {
                continue;
            }

            if (diff == 2) {
                return true;
            }
            // diff == 1
            if ((pos + 2) < poses.size()) {
                if ((poses[pos + 2] - next) == 1) {
                    return true;
                }
            }
        }
    }

    return false;
}

TEST(day5, part2) {

    struct args_t {
        std::string_view input;
        bool is_nice;
    };

    static constexpr args_t kArgs[] = {
        {
            .input = "qjhvhtzxzqqjkmpb",
            .is_nice = true,
        },
        {
            .input = "xxyxx",
            .is_nice = true,
        },
        {
            .input = "uurcxstgmygtbstg",
            .is_nice = false,
        },
        {
            .input = "ieodomkazucvgmuy",
            .is_nice = false,
        },

    };

    for (auto const& arg : kArgs) {
        SCOPED_TRACE(arg.input);
        ASSERT_EQ(arg.is_nice, is_nice_2(arg.input));
    }

    auto splitted = kInput | std::views::split('\n');
    auto count = std::ranges::fold_left(splitted, 0, [](auto sum, auto input) {
        if (is_nice_2({ input.begin(), input.end() })) {
            ++sum;
        }
        return sum;
    });

    ASSERT_EQ(55, count);
}
