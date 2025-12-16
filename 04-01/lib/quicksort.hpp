#ifndef QUICKSORT
#define QUICKSORT

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

namespace quicksort {

template <typename T>
void order(std::vector<T>& vector, std::size_t left, std::size_t right) {
    for (auto i = left + 1; i < right; ++i) {
        for (auto j = i; j > left; --j) {
            if (vector[j - 1] > vector[j]) {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

template <typename T>
std::size_t partition(std::vector<T>& vector, std::size_t left, std::size_t right) {
    auto middle_val = vector[std::midpoint(left, right)];

    auto i = left - 1;
    auto j = right;

    while (true) {
        while (vector[++i] < middle_val) {
        }
        while (vector[--j] > middle_val) {
        }
        if (i >= j) {
            return j;
        }
        std::swap(vector[i], vector[j]);
    }
}

template <typename T>
void split(std::vector<T>& vector, std::size_t left, std::size_t right) {
    if (right - left > 16) {
        auto middle = std::midpoint(left, right);

        if (vector[middle] < vector[left]) {
            std::swap(vector[middle], vector[left]);
        }
        if (vector[right - 1] < vector[left]) {
            std::swap(vector[right - 1], vector[left]);
        }
        if (vector[right - 1] < vector[middle]) {
            std::swap(vector[right - 1], vector[middle]);
        }

        auto p = partition(vector, left, right);
        split(vector, left, p);
        split(vector, p, right);

    } else {
        order(vector, left, right);
    }
}

template <typename T>
void quicksort(std::vector<T>& vector) {
    split(vector, 0, std::size(vector));
}

}  // namespace quicksort

#endif  // QUICKSORT