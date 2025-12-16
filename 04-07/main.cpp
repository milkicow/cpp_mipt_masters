#include <cassert>
#include <numeric>
#include <type_traits>

//////////////////////////////////////////////////////////////////////////////////////////

template <int N = 0, int D = 1>
struct Ratio {
    constexpr static auto num = N;
    constexpr static auto den = D;
};

//////////////////////////////////////////////////////////////////////////////////////////

template <int N, int D>
struct ReduceRatio {
    constexpr static auto gcd = std::gcd(N, D);
    constexpr static auto num = N / gcd;
    constexpr static auto den = D / gcd;

    using type = Ratio<num, den>;
};

template <typename R>
using reduced = typename ReduceRatio<R::num, R::den>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Sum {
    constexpr static auto num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto den = R1::den * R2::den;

    using type = typename ReduceRatio<num, den>::type;
};

template <typename R1, typename R2>
using sum = typename Sum<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Sub {
    using R2_neg = Ratio<-R2::num, R2::den>;
    using type = sum<R1, R2_neg>;
};

template <typename R1, typename R2>
using sub = typename Sub<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Mul {
    constexpr static auto num = R1::num * R2::num;
    constexpr static auto den = R1::den * R2::den;

    using type = typename ReduceRatio<num, den>::type;
};

template <typename R1, typename R2>
using mul = typename Mul<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Div {
    static_assert(R2::num != 0, "Division by zero in compile-time ratio");

    using R2_inv = Ratio<R2::den, R2::num>;
    using type = mul<R1, R2_inv>;
};

template <typename R1, typename R2>
using ratio_div = typename Div<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename R = Ratio<1>>
struct Duration {
    T x = T();
};

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator+(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    using ratio_t = sum<R1, R2>;

    auto x = (lhs.x * ratio_t::den / R1::den * R1::num + rhs.x * ratio_t::den / R2::den * R2::num);

    return Duration<decltype(x), ratio_t>{x};
}

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator-(Duration<T1, R1> const& lhs, Duration<T2, R2> const& rhs) {
    using R2_neg = Ratio<-R2::num, R2::den>;
    auto neg_rhs = Duration<T2, R2_neg>{rhs.x};

    return lhs + neg_rhs;
}

//////////////////////////////////////////////////////////////////////////////////////////

int main() {
    using R1 = Ratio<1, 2>;
    using R2 = Ratio<1, 3>;

    using SumResult = sum<R1, R2>;
    static_assert(SumResult::num == 5 && SumResult::den == 6, "Sum test failed");

    using SubResult = sub<R1, R2>;
    static_assert(SubResult::num == 1 && SubResult::den == 6, "Subtraction test failed");

    using MulResult = mul<R1, R2>;
    static_assert(MulResult::num == 1 && MulResult::den == 6, "Multiplication test failed");

    using DivResult = ratio_div<R1, R2>;
    static_assert(DivResult::num == 3 && DivResult::den == 2, "Division test failed");

    using R3 = Ratio<2, 4>;
    using ReducedR3 = reduced<R3>;
    static_assert(ReducedR3::num == 1 && ReducedR3::den == 2, "Reduction test failed");

    using R4 = Ratio<2, 3>;
    using R5 = Ratio<3, 4>;
    using MulResult2 = mul<R4, R5>;
    static_assert(MulResult2::num == 1 && MulResult2::den == 2,
                  "Multiplication with reduction test failed");

    auto x = 1, y = 2;

    Duration<int, Ratio<1, 2>> duration_1{x};
    Duration<int, Ratio<1, 3>> duration_2{y};

    auto duration_3 = duration_1 + duration_2;
    assert(duration_3.x == 7);

    auto duration_4 = duration_1 - duration_2;
    assert(duration_4.x == -1);

    // using TestDivByZero = ratio_div<R1, Ratio<0, 1>>;

    return 0;
}
