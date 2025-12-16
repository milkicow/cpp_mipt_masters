#include "quicksort.hpp"

#include <gtest/gtest.h>

#include <vector>

TEST(QuickSort, EmptySequence) {
    std::vector<int> vector{};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, OneElement) {
    std::vector<int> vector{0x52};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, TwoElementsSorted) {
    std::vector<int> vector{1, 2};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, TwoElementsNotSorted) {
    std::vector<int> vector{2, 1};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, TwoElementsEqual) {
    std::vector<int> vector{1, 1};
    quicksort::quicksort(vector);

    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(QuickSort, LongUnsortedSequence) {
    std::vector<int> vector{-5, 1, 12, 2, 8, 9, 5, 8, 11, -12, 37, 42, 1, 35};

    quicksort::quicksort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}
