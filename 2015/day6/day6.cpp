
#include <algorithm>
#include <array>
#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <system_error>

#include <gtest/gtest.h>

#include "input.hpp"

enum class command_t : std::uint8_t {
    toggle,
    turn_off,
    turn_on,
};

struct pos_t {
    std::size_t row;
    std::size_t column;
    auto operator<=>(pos_t const&) const = default;
};

struct line_t {
    command_t command;
    pos_t from;
    pos_t to;

    auto operator<=>(line_t const&) const = default;

    static line_t parse(std::string_view input) {
        line_t result {};
        result.command = parse_command(input);
        result.from = parse_from(input);
        result.to = parse_to(input);
        return result;
    }

    static pos_t parse_pos(std::string_view& input) {
        auto const space_pos = input.find(' ');
        auto const from = input.substr(0, space_pos);
        auto const comma_pos = from.find(',');
        auto const row = from.substr(0, comma_pos);
        auto const column = from.substr(comma_pos + 1, from.size());

        input.remove_prefix(space_pos + 1);
        return pos_t {
            .row = to_int(row),
            .column = to_int(column),
        };
    }

    static std::size_t to_int(std::string_view view) {
        std::size_t result = 0;
        if (auto [ptr, ec] = std::from_chars(view.begin(), view.end(), result); ptr != view.end() || ec != std::errc()) {
            throw std::invalid_argument("Invalid input");
        }

        return result;
    }

    static pos_t parse_from(std::string_view& input) {
        static constexpr std::string_view kThrough = "through";
        auto const ret = parse_pos(input);
        input.remove_prefix(kThrough.size() + 1);
        return ret;
    }

    static pos_t parse_to(std::string_view input) {
        return parse_pos(input);
    }

    static command_t parse_command(std::string_view& input) {
        static constexpr std::string_view kToggle = "toggle";
        static constexpr std::string_view kTurnOn = "turn on";
        static constexpr std::string_view kTurnOff = "turn off";

        if (input.starts_with(kToggle)) {
            input.remove_prefix(kToggle.size() + 1);
            return command_t::toggle;
        }

        if (input.starts_with(kTurnOff)) {
            input.remove_prefix(kTurnOff.size() + 1);
            return command_t::turn_off;
        }

        if (input.starts_with(kTurnOn)) {
            input.remove_prefix(kTurnOn.size() + 1);
            return command_t::turn_on;
        }

        throw std::invalid_argument("Invalid command");
    }
};

TEST(day6, parse_line) {
    struct args_t {
        std::string_view input;
        line_t expected;
    };

    static constexpr args_t kArgs[] = {
        {
            .input = "toggle 461,550 through 564,900",
            .expected = {
                .command = command_t::toggle,
                .from = {
                    .row = 461,
                    .column = 550,
                },
                .to = {
                    .row = 564,
                    .column = 900,
                },
            },
        }
    };

    for (auto const& arg : kArgs) {
        SCOPED_TRACE(arg.input);

        auto got = line_t::parse(arg.input);

        ASSERT_EQ(got, arg.expected);
    }
}

static std::array<line_t, 300> parse_commands() {
    constexpr auto kSplitByNl = std::ranges::views::split('\n');

    std::array<line_t, 300> cmds {};
    for (auto pos = 0U; auto const& line : kInput | kSplitByNl) {
        if (pos >= cmds.size()) {
            throw std::runtime_error("Too many lines");
        }
        cmds[pos] = line_t::parse({ line.begin(), line.end() });
        ++pos;
    }
    return cmds;
}

TEST(day6, part1) {
    using row_t = std::array<bool, 1000>;
    std::array<row_t, 1000> lights {};

    using manipulate_func_t = std::function<void(bool&)>;
    manipulate_func_t func {};
    for (auto const& [command, from, to] : parse_commands()) {
        switch (command) {
            case command_t::toggle:
                func = [](bool& value) { value = !value; };
                break;
            case command_t::turn_off:
                func = [](bool& value) { value = false; };
                break;
            case command_t::turn_on:
                func = [](bool& value) { value = true; };
                break;
            default:
                throw std::invalid_argument("Invalid command");
        }

        for (std::size_t row = from.row; row <= to.row; ++row) {
            for (auto column = from.column; column <= to.column; ++column) {
                func(lights[row][column]);
            }
        }
    }

    std::uint64_t lit = 0;
    for (auto& row : lights) {
        for (auto const& column : row) {
            if (column) {
                lit++;
            }
        }
    }
    ASSERT_EQ(543903, lit);
}

TEST(day6, part2) {
    using row_t = std::array<int, 1000>;
    std::array<row_t, 1000> lights {};

    using manipulate_func_t = std::function<void(int&)>;
    manipulate_func_t func {};
    for (auto const& [command, from, to] : parse_commands()) {
        switch (command) {
            case command_t::toggle:
                func = [](int& value) { value += 2; };
                break;
            case command_t::turn_off:
                func = [](int& value) {
                    --value;
                    value = std::max(0, value);
                };
                break;
            case command_t::turn_on:
                func = [](int& value) { value++; };
                break;
            default:
                throw std::invalid_argument("Invalid command");
        }

        for (auto row = from.row; row <= to.row; ++row) {
            for (auto column = from.column; column <= to.column; ++column) {
                func(lights[row][column]);
            }
        }
    }

    std::int64_t brightness = 0;
    for (auto& row : lights) {
        for (auto const& column : row) {
            brightness += column;
        }
    }
    ASSERT_EQ(14687245, brightness);
}
