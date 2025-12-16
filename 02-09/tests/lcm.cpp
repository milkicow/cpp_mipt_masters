#include "lcm.hpp"

#include <gtest/gtest.h>

#include <numeric>

TEST(LCMTest, BothImplementationsPositiveNumbers) {
    std::pair<int, int> test_cases[] = {{4, 6},   {5, 7},  {12, 18}, {21, 6},
                                        {15, 25}, {8, 12}, {36, 48}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), std::lcm(a, b));
        EXPECT_EQ(lcm::lcm_recursive(a, b), std::lcm(a, b));
    }
}

TEST(LCMTest, BothImplementationsNegativeNumbers) {
    std::pair<int, int> test_cases[] = {{-4, 6},   {5, -7},  {-12, -18},
                                        {-15, 25}, {8, -12}, {-36, -48}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), std::lcm(a, b));
        EXPECT_EQ(lcm::lcm_recursive(a, b), std::lcm(a, b));
    }
}

TEST(LCMTest, BothImplementationsEdgeCases) {
    std::pair<int, int> test_cases[] = {{0, 5},  {5, 0}, {0, 0},   {0, -5},
                                        {-5, 0}, {1, 1}, {1, 100}, {100, 1}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), std::lcm(a, b));
        EXPECT_EQ(lcm::lcm_recursive(a, b), std::lcm(a, b));
    }
}

TEST(LCMTest, BothImplementationsLargeNumbers) {
    std::pair<int, int> test_cases[] = {{1000000, 999999}, {123456, 7890}, {99999, 88888}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), std::lcm(a, b));
        EXPECT_EQ(lcm::lcm_recursive(a, b), std::lcm(a, b));
    }
}

TEST(LCMTest, BothImplementationsPrimeNumbers) {
    std::pair<int, int> test_cases[] = {{17, 19}, {23, 29}, {31, 37}, {41, 43}, {2, 3}, {97, 101}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), std::lcm(a, b));
        EXPECT_EQ(lcm::lcm_recursive(a, b), std::lcm(a, b));
    }
}

// Test consistency between iterative and recursive LCM implementations
TEST(LCMTest, ConsistencyBetweenLCMImplementations) {
    std::pair<int, int> test_cases[] = {
        {4, 6}, {12, 18}, {21, 6},           {-4, 6},        {5, -7},  {-12, -18}, {0, 5},
        {5, 0}, {0, 0},   {1000000, 999999}, {123456, 7890}, {17, 19}, {2, 3}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), lcm::lcm_recursive(a, b))
            << "LCM implementations inconsistent for (" << a << ", " << b << ")";
    }
}

TEST(LCMTest, DirectCalculationEquivalence) {
    std::pair<int, int> test_cases[] = {{4, 6},   {12, 18},  {21, 6}, {15, 25},
                                        {17, 19}, {100, 75}, {8, 12}};

    for (const auto& [a, b] : test_cases) {
        if (a != 0 && b != 0) {
            int direct_lcm = std::abs(a * b) / std::gcd(a, b);

            EXPECT_EQ(lcm::lcm_iterative(a, b), direct_lcm)
                << "Iterative LCM doesn't match direct calculation for (" << a << ", " << b << ")";

            EXPECT_EQ(lcm::lcm_recursive(a, b), direct_lcm)
                << "Recursive LCM doesn't match direct calculation for (" << a << ", " << b << ")";
        }
    }
}

// Test commutative property for both LCM implementations
TEST(LCMTest, CommutativeProperty) {
    std::pair<int, int> test_cases[] = {{4, 6},  {12, 18},   {21, 6},           {-4, 6},
                                        {5, -7}, {-12, -18}, {1000000, 999999}, {17, 19}};

    for (const auto& [a, b] : test_cases) {
        EXPECT_EQ(lcm::lcm_iterative(a, b), lcm::lcm_iterative(b, a))
            << "Iterative LCM not commutative for (" << a << ", " << b << ")";

        EXPECT_EQ(lcm::lcm_recursive(a, b), lcm::lcm_recursive(b, a))
            << "Recursive LCM not commutative for (" << a << ", " << b << ")";

        EXPECT_EQ(lcm::lcm_iterative(a, b), lcm::lcm_recursive(b, a))
            << "Cross-implementation LCM not commutative for (" << a << ", " << b << ")";
    }
}