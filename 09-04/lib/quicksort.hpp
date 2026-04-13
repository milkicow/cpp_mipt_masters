#ifndef QUICKSORT
#define QUICKSORT

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <vector>

namespace quicksort {

template <typename RandomIt>
void order(RandomIt first, RandomIt last) {
    for (auto i = std::next(first); i != last; ++i) {
        for (auto j = i; j != first; --j) {
            auto prev = std::prev(j);
            if (*prev > *j) {
                std::iter_swap(j, prev);
            }
        }
    }
}

template <typename RandomIt>
RandomIt partition(RandomIt first, RandomIt last) {
    auto size = std::distance(first, last);
    auto middle = std::next(first, size / 2);
    auto middle_val = *middle;

    auto i = first;
    auto j = std::prev(last);

    while (true) {
        while (*i < middle_val) {
            std::advance(i, 1);
        }
        while (*j > middle_val) {
            std::advance(j, -1);
        }
        if (std::distance(i, j) <= 0) {
            return j;
        }
        std::iter_swap(i, j);
    }
}

template <typename RandomIt>
void split(RandomIt first, RandomIt last) {
    auto size = std::distance(first, last);

    if (size > 16) {
        auto middle = std::next(first, size / 2);
        auto last_prev = std::prev(last);

        if (*middle < *first) {
            std::iter_swap(middle, first);
        }
        if (*last_prev < *first) {
            std::iter_swap(last_prev, first);
        }
        if (*last_prev < *middle) {
            std::iter_swap(last_prev, middle);
        }

        auto p = partition(first, last);
        split(first, std::next(p));
        split(std::next(p), last);

    } else {
        order(first, last);
    }
}

template <typename RandomIt>
void quicksort(RandomIt first, RandomIt last) {
    if (first != last && std::distance(first, last) > 1) {
        split(first, last);
    }
}

template <typename T>
void quicksort(std::vector<T>& vector) {
    quicksort(vector.begin(), vector.end());
}

}  // namespace quicksort

#endif  // QUICKSORT