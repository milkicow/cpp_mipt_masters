#include <limits>

template <int N>
struct Fibonacci {
    static_assert(N >= 0, "N must be non-negative");

    static constexpr bool check_overflow() {
        if constexpr (N > 1) {
            constexpr long long fib_n_minus_1 = Fibonacci<N - 1>::value;
            constexpr long long fib_n_minus_2 = Fibonacci<N - 2>::value;
            constexpr long long sum = fib_n_minus_1 + fib_n_minus_2;

            return sum > static_cast<long long>(std::numeric_limits<int>::max());
        }
        return false;
    }

    static_assert(!check_overflow(), "Fibonacci computation would cause integer overflow");

    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

template <int N>
inline constexpr int Fibonacci_v = Fibonacci<N>::value;

int main() {
    static_assert(Fibonacci_v<0> == 0, "Fibonacci_v<0> should be 0");
    static_assert(Fibonacci_v<1> == 1, "Fibonacci_v<1> should be 1");
    static_assert(Fibonacci_v<2> == 1, "Fibonacci_v<2> should be 1");
    static_assert(Fibonacci_v<5> == 5, "Fibonacci_v<5> should be 5");
    static_assert(Fibonacci_v<10> == 55, "Fibonacci_v<10> should be 55");

    static_assert(Fibonacci<20>::value == 6765, "Fibonacci(20) should be 6765");
    static_assert(Fibonacci<8>::value == Fibonacci<6>::value + Fibonacci<7>::value);

    return 0;
}
