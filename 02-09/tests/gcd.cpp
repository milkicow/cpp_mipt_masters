#include "gcd.hpp"

#include <gtest/gtest.h>

#include <numeric>

TEST(GCDTest, PositiveNumbers) {
    std::pair<int, int> test_cases[] = {{48, 18},   {56, 42}, {17, 13},  {100, 75},
                                        {1, 1},     {1, 100}, {100, 1},  {123456, 7890},
                                        {999, 111}, {81, 27}, {255, 85}, {144, 89}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, NegativeNumbers) {
    std::pair<int, int> test_cases[] = {{-48, 18},  {56, -42}, {-100, -75}, {-17, 13}, {17, -13},
                                        {-17, -13}, {-1, 1},   {1, -1},     {-1, -1}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, EqualNumbers) {
    std::pair<int, int> test_cases[] = {{15, 15}, {1, 1},       {100, 100}, {-15, -15},
                                        {-1, -1}, {-100, -100}, {0, 0}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, LargeNumbers) {
    std::pair<int, int> test_cases[] = {{2147483647, 2147483646},
                                        {1000000000, 999999999},
                                        {123456789, 987654321},
                                        {999999999, 888888888}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

// Test consistency between iterative and recursive implementations
TEST(GCDTest, ConsistencyBetweenImplementations) {
    std::pair<int, int> test_cases[] = {{48, 18}, {56, 42},       {-100, -75},
                                        {0, 5},   {123456, 7890}, {2147483647, 2147483646}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(gcd::gcd_iterative(a, b), gcd::gcd_recursive(a, b));
    }
}

TEST(GCDTest, EdgeCasesWithZeros) {
    std::pair<int, int> test_cases[] = {{0, 5}, {5, 0}, {0, 0}, {0, -5}, {-5, 0}, {0, 1}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, PrimeNumbers) {
    std::pair<int, int> test_cases[] = {{17, 19}, {23, 29}, {31, 37}, {41, 43}, {2, 3}, {97, 101}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, ConsecutiveNumbers) {
    std::pair<int, int> test_cases[] = {{15, 16}, {100, 101}, {999, 1000}, {1, 2}, {42, 43}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}

TEST(GCDTest, WithOne) {
    std::pair<int, int> test_cases[] = {{1, 1},    {1, 100},  {100, 1},
                                        {1, -100}, {-100, 1}, {-1, 100}};

    for (const auto& [a, b] : test_cases) {
        int std_result = std::gcd(a, b);
        EXPECT_EQ(gcd::gcd_iterative(a, b), std_result);
        EXPECT_EQ(gcd::gcd_recursive(a, b), std_result);
    }
}
