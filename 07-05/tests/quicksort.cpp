#include "quicksort.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(QuickSort, numberSequence) {
    std::vector<int> vector{-5, 1, 12, 2, 8, 9, 5, 8, 11, -12, 37, 42, 1, 35};
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, stringSequence) {
    std::vector<std::string> vector{"banana", "apple", "orange", "kiwi", "grape"};
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, structSequence) {
    struct Point {
        int x;
        int y;
        auto operator<=>(const Point& other) const = default;
    };
    std::vector<Point> vector{{1, 2}, {3, 4}, {0, 0}, {5, 1}, {2, 2}};
    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

// Дополнительные тесты с различными порогами
TEST(QuickSort, customThreshold1) {
    std::vector<double> v(10000);
    std::iota(v.begin(), v.end(), 0.0);
    std::reverse(v.begin(), v.end());
    quicksort::quicksort(v, 1);
    EXPECT_TRUE(std::ranges::is_sorted(v));
}

TEST(QuickSort, customThreshold16) {
    std::vector<double> v(10000);
    std::iota(v.begin(), v.end(), 0.0);
    std::reverse(v.begin(), v.end());
    quicksort::quicksort(v, 16);
    EXPECT_TRUE(std::ranges::is_sorted(v));
}

TEST(QuickSort, customThreshold512) {
    std::vector<double> v(10000);
    std::iota(v.begin(), v.end(), 0.0);
    std::reverse(v.begin(), v.end());
    quicksort::quicksort(v, 512);
    EXPECT_TRUE(std::ranges::is_sorted(v));
}
