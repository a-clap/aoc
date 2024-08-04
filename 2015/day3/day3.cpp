#include <set>
#include <string_view>

#include <gtest/gtest.h>

#include "input.hpp"

struct location_t {
    int row;
    int column;

    constexpr auto operator<=>(location_t const&) const = default;
};

static std::set<location_t> get_locations_1(std::string_view const input) {
    std::set<location_t> locations;

    location_t location(0, 0);
    locations.insert(location);

    for (auto const dir : input) {
        if (dir == '>') {
            ++location.row;
        } else if (dir == '<') {
            --location.row;
        } else if (dir == '^') {
            ++location.column;
        } else if (dir == 'v') {
            --location.column;
        }

        locations.insert(location);
    }

    return locations;
}

static std::set<location_t> get_locations_2(std::string_view const input) {
    std::set<location_t> locations;

    location_t santa(0, 0);
    locations.insert(santa);

    location_t robosanta(0, 0);

    constexpr auto kMakeAMove = [](location_t& location) {
        return [&location](char const dir) {
            if (dir == '>') {
                ++location.row;
            } else if (dir == '<') {
                --location.row;
            } else if (dir == '^') {
                ++location.column;
            } else if (dir == 'v') {
                --location.column;
            }

            return location;
        };
    };

    for (auto pos = 0; auto const dir : input) {
        auto const next = [&]() {
            if (pos % 2 == 0) {
                return kMakeAMove(santa);
            }
            return kMakeAMove(robosanta);
        }()(dir);

        locations.insert(next);
        ++pos;
    }

    return locations;
}

TEST(day3, input_1) {

    struct arg_t {
        std::string_view input;
        int houses;
    };

    arg_t const args[] = {
        {
            .input = ">",
            .houses = 2,
        },
        {
            .input = "^>v<",
            .houses = 4,
        },
        {
            .input = "^v^v^v^v^v",
            .houses = 2,
        }
    };
    for (auto const [input, houses] : args) {
        SCOPED_TRACE(input);

        auto const answer = get_locations_1(input);
        ASSERT_EQ(answer.size(), houses);
    }

    auto answer_1 = get_locations_1(kInput).size();
    ASSERT_EQ(answer_1, 2081);
}

TEST(day3, input_2) {

    struct arg_t {
        std::string_view input;
        int houses;
    };

    arg_t const args[] = {
        {
            .input = "^v",
            .houses = 3,
        },
        {
            .input = "^>v<",
            .houses = 3,
        },
        {
            .input = "^v^v^v^v^v",
            .houses = 11,
        }
    };
    for (auto const [input, houses] : args) {
        SCOPED_TRACE(input);

        auto const answer = get_locations_2(input);
        ASSERT_EQ(answer.size(), houses);
    }

    auto answer_1 = get_locations_2(kInput).size();
    ASSERT_EQ(answer_1, 2341);
}
