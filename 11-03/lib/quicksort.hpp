#ifndef QUICKSORT
#define QUICKSORT

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <vector>

namespace quicksort {

template <typename RandomIt, typename Compare>
void order(RandomIt first, RandomIt last, Compare comp) {
    for (auto i = std::next(first); i != last; ++i) {
        for (auto j = i; j != first; --j) {
            auto prev = std::prev(j);
            if (comp(*j, *prev)) {
                std::iter_swap(j, prev);
            }
        }
    }
}

template <typename RandomIt, typename Compare>
RandomIt partition(RandomIt first, RandomIt last, Compare comp) {
    auto size = std::distance(first, last);
    auto middle = std::next(first, size / 2);
    auto middle_val = *middle;

    auto i = first;
    auto j = std::prev(last);

    while (true) {
        while (comp(*i, middle_val)) {
            std::advance(i, 1);
        }
        while (comp(middle_val, *j)) {
            std::advance(j, -1);
        }
        if (std::distance(i, j) <= 0) {
            return j;
        }
        std::iter_swap(i, j);
    }
}

template <typename RandomIt, typename Compare>
void split(RandomIt first, RandomIt last, Compare comp) {
    auto size = std::distance(first, last);

    if (size > 16) {
        auto middle = std::next(first, size / 2);
        auto last_prev = std::prev(last);

        if (comp(*middle, *first)) {
            std::iter_swap(middle, first);
        }
        if (comp(*last_prev, *first)) {
            std::iter_swap(last_prev, first);
        }
        if (comp(*last_prev, *middle)) {
            std::iter_swap(last_prev, middle);
        }

        auto p = quicksort::partition(first, last, comp);
        split(first, std::next(p), comp);
        split(std::next(p), last, comp);

    } else {
        order(first, last, comp);
    }
}

template <typename RandomIt, typename Compare>
void quicksort(RandomIt first, RandomIt last, Compare comp) {
    if (first != last && std::distance(first, last) > 1) {
        split(first, last, comp);
    }
}

template <typename RandomIt>
void quicksort(RandomIt first, RandomIt last) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    quicksort(first, last, std::less<value_type>{});
}

template <typename T>
void quicksort(std::vector<T>& vector) {
    quicksort(vector.begin(), vector.end());
}


}  // namespace quicksort

#endif  // QUICKSORT