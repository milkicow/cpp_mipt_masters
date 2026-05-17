#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <ranges>
#include <vector>

namespace ranges_utils {

template <std::ranges::input_range R, typename OutIt, typename Transform, typename Predicate>
OutIt transform_if(R&& range, OutIt out, Transform transform, Predicate pred) {
    std::vector<std::ranges::range_value_t<R>> tmp;
    std::ranges::copy_if(range, std::back_inserter(tmp), pred);
    return std::ranges::transform(tmp, out, transform).out;
}

inline double mae(const std::vector<double>& actual, const std::vector<double>& predicted) {
    return std::transform_reduce(actual.begin(), actual.end(), predicted.begin(), 0.0,
                                 std::plus<>{},
                                 [](double a, double b) { return std::abs(a - b); }) /
           actual.size();
}

inline double mse(const std::vector<double>& actual, const std::vector<double>& predicted) {
    return std::transform_reduce(actual.begin(), actual.end(), predicted.begin(), 0.0,
                                 std::plus<>{},
                                 [](double a, double b) { return (a - b) * (a - b); }) /
           actual.size();
}

}  // namespace ranges_utils
