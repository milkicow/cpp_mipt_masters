#include "quicksort.hpp"

#include <gtest/gtest.h>

#include <vector>

TEST(QuickSort, numberSequence) {
    std::vector<int> vector{-5, 1, 12, 2, 8, 9, 5, 8, 11, -12, 37, 42, 1, 35};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, numberSequencePartial) {
    std::vector<int> vector{-5, 1, 12, 2, 8, 9, 5, 8, 11, -12, 37, 42, 1, 35};

    auto start_it = vector.begin() + 2;
    auto end_it = vector.end();

    quicksort::quicksort(start_it, end_it);
    EXPECT_TRUE(std::ranges::is_sorted(start_it, end_it));

    EXPECT_TRUE(vector[0] == -5);
    EXPECT_TRUE(vector[1] == 1);
}

TEST(QuickSort, stringSequence) {
    std::vector<std::string> vector{"banana", "apple", "orange", "kiwi", "grape"};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, stringSequencePartial) {
    std::vector<std::string> vector{"banana", "apple", "orange", "kiwi", "grape"};

    auto start_it = std::next(vector.begin());
    auto end_it = std::prev(vector.end());

    quicksort::quicksort(start_it, end_it);
    EXPECT_TRUE(std::ranges::is_sorted(start_it, end_it));
    EXPECT_TRUE(vector.front() == "banana");
    EXPECT_TRUE(vector.back() == "grape");
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

TEST(QuickSort, structSequencePartial) {
    struct Point {
        int x;
        int y;

        auto operator<=>(const Point& other) const = default;
    };

    std::vector<Point> vector{{1, 2}, {3, 4}, {0, 0}, {5, 1}, {2, 2}};

    auto start_it = vector.begin();
    auto end_it = vector.end() - 2;

    quicksort::quicksort(start_it, end_it);
    EXPECT_TRUE(std::ranges::is_sorted(start_it, end_it));

    EXPECT_TRUE((vector.back() == Point{2, 2}));
    EXPECT_TRUE((*(vector.end() - 2) == Point{5, 1}));
}

static bool greater_int(int a, int b) { return a > b; }

TEST(QuickSort, customFunctionComparator) {
    std::vector<int> v{3, 1, 4, 2, 7, 6};

    quicksort::quicksort(v.begin(), v.end(), greater_int);
    EXPECT_TRUE(std::ranges::is_sorted(v, greater_int));
}

TEST(QuickSort, stdLessComparator) {
    std::vector<int> v{3, 1, 4, 2, 7, 6};

    quicksort::quicksort(v.begin(), v.end(), std::greater<int>{});
    EXPECT_TRUE(std::ranges::is_sorted(v, std::greater<int>{}));
}

TEST(QuickSort, lambdaComparator) {
    std::vector<int> v{3, 1, 4, 2, 7, 6, 107, 16};

    auto cmp = [](int a, int b) { return (a % 10) < (b % 10); };

    quicksort::quicksort(v.begin(), v.end(), cmp);
    EXPECT_TRUE(std::ranges::is_sorted(v, cmp));
}
