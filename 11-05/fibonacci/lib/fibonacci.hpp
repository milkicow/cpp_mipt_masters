#pragma once

#include <iterator>
#include <ranges>
#include <utility>

class Fibonacci : public std::ranges::view_interface<Fibonacci> {
   private:
    class Iterator {
       public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = const int&;

        Iterator() : a_(0), b_(1) {}

        const int& operator*() const { return a_; }
        const int* operator->() const { return &a_; }

        Iterator& operator++() {
            a_ = std::exchange(b_, a_ + b_);
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { return a_ == other.a_ && b_ == other.b_; }

       private:
        int a_;
        int b_;
    };

   public:
    Fibonacci() = default;

    Iterator begin() const { return Iterator{}; }
    std::unreachable_sentinel_t end() const { return std::unreachable_sentinel; }
};
