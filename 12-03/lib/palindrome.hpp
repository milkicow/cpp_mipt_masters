#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

namespace palindrome {

inline std::string_view longest(std::string_view s) {
    const auto n = s.size();
    if (n == 0) return s;

    std::vector<bool> dp(n * n, false);

    std::size_t start = 0;
    std::size_t max_len = 1;

    for (std::size_t i = 0; i < n; ++i) {
        dp[i * n + i] = true;
    }

    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (s[i] == s[i + 1]) {
            dp[i * n + i + 1] = true;
            start = i;
            max_len = 2;
        }
    }

    for (std::size_t len = 3; len <= n; ++len) {
        for (std::size_t i = 0; i + len <= n; ++i) {
            const std::size_t j = i + len - 1;
            if (s[i] == s[j] && dp[(i + 1) * n + (j - 1)]) {
                dp[i * n + j] = true;
                if (len > max_len) {
                    start = i;
                    max_len = len;
                }
            }
        }
    }

    return s.substr(start, max_len);
}

}  // namespace palindrome
