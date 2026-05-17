#include <gtest/gtest.h>

#include <vector>

#include "fibonacci.hpp"

TEST(Fibonacci, firstTenElements) {
    Fibonacci fib;
    std::vector<int> result;
    for (auto x : fib | std::views::take(10)) result.push_back(x);
    EXPECT_EQ((std::vector<int>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}), result);
}

TEST(Fibonacci, filterEven) {
    Fibonacci fib;
    std::vector<int> result;
    for (auto x :
         fib | std::views::filter([](int x) { return x % 2 == 0; }) | std::views::take(4)) {
        result.push_back(x);
    }
    EXPECT_EQ((std::vector<int>{0, 2, 8, 34}), result);
}

TEST(Fibonacci, dropFirst) {
    Fibonacci fib;
    std::vector<int> result;
    for (auto x : fib | std::views::drop(5) | std::views::take(5)) result.push_back(x);
    EXPECT_EQ((std::vector<int>{5, 8, 13, 21, 34}), result);
}
