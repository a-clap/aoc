#include <algorithm>
#include <cctype>
#include <cstdint>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include "input.hpp"

using wires_t = std::unordered_map<std::string, std::uint16_t>;
using command_t = std::function<bool(wires_t& wires)>;

namespace {

bool is_number(std::string const& str) {
    return std::ranges::all_of(str.begin(), str.end(), [](unsigned char maybe_digit) { return isdigit(maybe_digit); });
}

std::uint16_t to_number(std::string const& str) {
    return static_cast<std::uint16_t>(std::stoul(str));
}

command_t make_command(std::string const& command, std::string const& first, std::string const& second, std::string const& dst) {  // NOLINT(*-easily-swappable-parameters)
    return [=](wires_t& wires) -> bool {
        auto const first_is_number = is_number(first);
        auto const second_is_number = is_number(second);

        if (!first_is_number && !wires.contains(first)) {
            return false;
        }

        if (!second_is_number && !wires.contains(second)) {
            return false;
        }

        std::uint16_t const first_value = first_is_number ? to_number(first) : wires.at(first);
        std::uint16_t const second_value = second_is_number ? to_number(second) : wires.at(second);

        if (command == "AND") {
            wires[dst] = first_value & second_value;
            return true;
        }
        if (command == "OR") {
            wires[dst] = first_value | second_value;
            return true;
        }
        if (command == "RSHIFT") {
            wires[dst] = first_value >> second_value;
            return true;
        }

        if (command == "LSHIFT") {
            wires[dst] = static_cast<std::uint16_t>(first_value << second_value);
            return true;
        }
        throw std::invalid_argument("Invalid logic gate");
    };
}

command_t parse_line(std::string_view line) {
    std::vector<std::string> tokens;

    for (auto token : line | std::views::split(' ')) {
        tokens.emplace_back(token.begin(), token.end());
    }

    auto const& size = tokens.size();
    auto const dst = tokens.back();

    if (size == 5) {
        // rshift, lshift, and, or
        return make_command(tokens[1], tokens[0], tokens[2], dst);
    }
    if (size == 4) {
        // NOT
        auto const& from = tokens[1];
        return [=](wires_t& wires) -> bool {
            if (wires.contains(from)) {
                wires[dst] = ~wires.at(from);
                return true;
            }

            if (is_number(from)) {
                wires[dst] = !to_number(from);
                return true;
            }

            return false;
        };
    }

    if (size == 3) {
        // move value or reg
        auto const& from = tokens[0];
        return [=](wires_t& wires) -> bool {
            if (wires.contains(from)) {
                wires[dst] = wires.at(from);
                return true;
            }

            if (is_number(from)) {
                wires[dst] = to_number(from);
                return true;
            }

            return false;
        };
    }
    throw std::runtime_error("unknown size");
}
}  // namespace

TEST(day7, parse_line) {

    std::vector<command_t> commands = {};
    for (auto const& line : kInput | std::views::split('\n')) {
        std::string_view const linesv = { line.begin(), line.end() };
        commands.emplace_back(parse_line(linesv));
    }

    auto handle_commands = [&] -> wires_t {
        wires_t wires;
        std::vector results(commands.size(), false);
        for (auto i = 0U; i < results.size();) {
            for (auto pos = 0U; pos < results.size(); ++pos) {
                if (!results[pos]) {
                    results[pos] = commands[pos](wires);
                    if (results[pos]) {
                        ++i;
                    }
                }
            }
        }
        return wires;
    };

    static constexpr auto kFirstAnswer = 46065;
    auto const first = handle_commands();
    ASSERT_EQ(kFirstAnswer, first.at("a"));

    static constexpr auto kSecondAnswer = 14134;
    commands.emplace_back(parse_line("46065 -> b"));
    auto const second = handle_commands();
    ASSERT_EQ(kSecondAnswer, second.at("a"));
}
