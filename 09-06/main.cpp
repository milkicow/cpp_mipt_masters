#include <boost/iterator/iterator_facade.hpp>
#include <iostream>
#include <iterator>

class Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    Iterator() : a_(0), b_(1) {}

    reference operator*() const { return a_; }

    Iterator& operator++() {
        advance();
        return *this;
    }

    Iterator operator++(int) {
        Iterator tmp = *this;
        advance();
        return tmp;
    }

    bool operator==(const Iterator& other) const { return a_ == other.a_ && b_ == other.b_; }

   private:
    int a_;
    int b_;

    void advance() { a_ = std::exchange(b_, a_ + b_); }
};

class FibonacciIterator
    : public boost::iterator_facade<FibonacciIterator, const int, boost::forward_traversal_tag> {
   public:
    FibonacciIterator() : a_(0), b_(1) {}

   private:
    friend class boost::iterator_core_access;

    const int& dereference() const { return a_; }

    void increment() { a_ = std::exchange(b_, a_ + b_); }

    bool equal(const FibonacciIterator& other) const { return a_ == other.a_ && b_ == other.b_; }

    int a_;
    int b_;
};

template <typename Iter>
void print_fibonacci(Iter it, int count, std::string label) {
    std::cout << label << ":\n  ";
    for (int i = 0; i < count; ++i, ++it) {
        std::cout << *it;
        if (i + 1 < count) std::cout << ", ";
    }
    std::cout << "\n\n";
}

int main() {
    constexpr int N = 15;

    print_fibonacci(Iterator{}, N, "Iterator");
    print_fibonacci(FibonacciIterator{}, N, "Boost::iterator_facade");

    {
        std::cout << "postfix increment: Iterator:\n";
        Iterator it;
        for (int i = 0; i < 8; ++i) {
            std::cout << "  *it++ = " << *it++ << "\n";
        }
        std::cout << "\n";
    }

    {
        std::cout << "std::advance Boost Iterator:\n";
        FibonacciIterator it;
        std::advance(it, 10);
        std::cout << "  F(10) = " << *it << "\n\n";
    }

    {
        Iterator a, b;
        std::cout << "operator == :\n";
        std::cout << "  a == b " << std::boolalpha << (a == b) << "\n";
        ++a;
        std::cout << "  ++a\n";
        std::cout << "  a == b " << std::boolalpha << (a == b) << "\n\n";
    }

    return 0;
}