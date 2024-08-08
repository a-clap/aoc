#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include <gtest/gtest.h>

#include <fmt/core.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/types.h>

#include "input.hpp"

namespace {
void calculate_md5(char const* buf, std::size_t const size, unsigned char* dst) {
    auto md5_digest_len = static_cast<unsigned int>(EVP_MD_size(EVP_md5()));

    // MD5_Init
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr);

    // MD5_Update
    EVP_DigestUpdate(mdctx, buf, size);

    // MD5_Final
    EVP_DigestFinal_ex(mdctx, dst, &md5_digest_len);
    EVP_MD_CTX_free(mdctx);
}
}  // namespace

TEST(test, part_1) {
    std::array<unsigned char, MD5_DIGEST_LENGTH> res {};

    auto const leading_zeroes = [&](std::size_t const zeroes) -> bool {
        std::size_t const twice = zeroes / 2;
        auto const rest = zeroes - (twice * 2);

        for (auto beg = 0U; beg < twice; ++beg) {
            if (res[beg] != 0) {
                return false;
            }
        }

        if (rest) {
            std::uint8_t const value = res[twice];
            return (value & 0xf0U) == 0U;
        }

        return true;
    };

    std::size_t first { 0 };
    std::size_t second { 0 };
    std::size_t candidate { 0 };

    while (first == 0 || second == 0) {
        auto cand = fmt::format("{}{}", kInput, candidate);
        calculate_md5(cand.data(), cand.size(), res.data());

        if (first == 0 && leading_zeroes(5)) {
            first = candidate;
        } else if (second == 0 && leading_zeroes(6)) {
            second = candidate;
        }

        ++candidate;
    }

    ASSERT_EQ(117946, first);
    ASSERT_EQ(3938038, second);
}
