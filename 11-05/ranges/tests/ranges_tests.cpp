#include <gtest/gtest.h>

#include <random>
#include <string>
#include <vector>

#include "ranges_utils.hpp"

TEST(RangesAlgorithms, replace) {
    std::vector<int> v{1, 2, 3, 2, 4};
    std::ranges::replace(v, 2, 99);
    EXPECT_EQ((std::vector<int>{1, 99, 3, 99, 4}), v);
}

TEST(RangesAlgorithms, fill) {
    std::vector<int> v(5);
    std::ranges::fill(v, 7);
    EXPECT_EQ((std::vector<int>{7, 7, 7, 7, 7}), v);
}

TEST(RangesAlgorithms, unique) {
    std::vector<int> v{1, 1, 2, 3, 3, 3, 4};
    auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);
    EXPECT_EQ((std::vector<int>{1, 2, 3, 4}), v);
}

TEST(RangesAlgorithms, rotate) {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::ranges::rotate(v, v.begin() + 2);
    EXPECT_EQ((std::vector<int>{3, 4, 5, 1, 2}), v);
}

TEST(RangesAlgorithms, sample) {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> out(3);
    std::mt19937 rng{42};
    std::ranges::sample(v, out.begin(), 3, rng);
    EXPECT_EQ(3u, out.size());
    for (auto x : out) {
        EXPECT_TRUE(std::ranges::contains(v, x));
    }
}

TEST(TransformIf, squareEvens) {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::vector<int> out;
    ranges_utils::transform_if(
        v, std::back_inserter(out), [](int x) { return x * x; }, [](int x) { return x % 2 == 0; });
    EXPECT_EQ((std::vector<int>{4, 16, 36}), out);
}

TEST(TransformIf, negatePositives) {
    std::vector<int> v{-3, -1, 0, 2, 4};
    std::vector<int> out;
    ranges_utils::transform_if(
        v, std::back_inserter(out), [](int x) { return -x; }, [](int x) { return x > 0; });
    EXPECT_EQ((std::vector<int>{-2, -4}), out);
}

TEST(ErrorMetrics, mae) {
    std::vector<double> actual{1.0, 2.0, 3.0, 4.0};
    std::vector<double> predicted{1.5, 2.5, 3.5, 4.5};
    EXPECT_DOUBLE_EQ(0.5, ranges_utils::mae(actual, predicted));
}

TEST(ErrorMetrics, mse) {
    std::vector<double> actual{1.0, 2.0, 3.0, 4.0};
    std::vector<double> predicted{1.5, 2.5, 3.5, 4.5};
    EXPECT_DOUBLE_EQ(0.25, ranges_utils::mse(actual, predicted));
}

TEST(Views, filter) {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::vector<int> result;
    for (auto x : v | std::views::filter([](int x) { return x % 2 == 0; })) result.push_back(x);
    EXPECT_EQ((std::vector<int>{2, 4, 6}), result);
}

TEST(Views, drop) {
    std::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> result;
    for (auto x : v | std::views::drop(2)) result.push_back(x);
    EXPECT_EQ((std::vector<int>{3, 4, 5}), result);
}

TEST(Views, join) {
    std::vector<std::vector<int>> v{{1, 2}, {3, 4}, {5, 6}};
    std::vector<int> result;
    for (auto x : v | std::views::join) result.push_back(x);
    EXPECT_EQ((std::vector<int>{1, 2, 3, 4, 5, 6}), result);
}

TEST(Views, zip) {
    std::vector<int> a{1, 2, 3};
    std::vector<std::string> b{"x", "y", "z"};
    std::vector<std::pair<int, std::string>> result;
    for (auto [x, y] : std::views::zip(a, b)) result.emplace_back(x, y);
    EXPECT_EQ((std::vector<std::pair<int, std::string>>{{1, "x"}, {2, "y"}, {3, "z"}}), result);
}

TEST(Views, stride) {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> result;
    for (auto x : v | std::views::stride(3)) result.push_back(x);
    EXPECT_EQ((std::vector<int>{1, 4, 7}), result);
}
