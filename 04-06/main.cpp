#include <array>
#include <cmath>
#include <concepts>
#include <ranges>

template <std::floating_point T>
consteval T my_abs(T x) {
    return x < 0 ? -x : x;
}

template <std::floating_point T>
consteval T compute_exp(T epsilon) {
    T exp = 1;
    T elem = 1;
    for (const auto n : std::views::iota(0)) {
        elem /= (n + 1);
        if (my_abs(elem) < epsilon) {
            break;
        }
        exp += elem;

        if (n > 10000) {
            break;
        }
    }
    return exp;
}

template <std::floating_point T>
consteval T compute_pi(T epsilon) {
    T quarter_pi = 1;
    T sign = -1;

    for (const auto n : std::views::iota(1)) {
        T denominator = 2 * n + 1;
        T elem = sign / denominator;

        if (my_abs(elem) < epsilon) {
            break;
        }

        quarter_pi += elem;
        sign = -sign;

        if (n > 10000) {
            break;
        }
    }

    return quarter_pi * 4;
}

int main() {
    constexpr auto kEpsilonExp = std::array{2.0, 1.0, 0.5, 0.1, 0.04, 0.008};

    static_assert(compute_exp(kEpsilonExp[0]) == 1);
    static_assert(compute_exp(kEpsilonExp[1]) == 1 + 1);
    static_assert(compute_exp(kEpsilonExp[2]) == 1 + 1 + 0.5);
    static_assert(compute_exp(kEpsilonExp[3]) == 1 + 1 + 0.5 + (1.0 / 6));
    static_assert(compute_exp(kEpsilonExp[4]) == 1 + 1 + 0.5 + (1.0 / 6) + (1.0 / 24));
    static_assert(compute_exp(kEpsilonExp[5]) ==
                  1 + 1 + 0.5 + (1.0 / 6) + (1.0 / 24) + (1.0 / 120));

    constexpr auto kEpsilonPi = std::array{3.0, 1.0, 0.3, 0.2, 0.14, 0.11};

    static_assert(compute_pi(kEpsilonPi[0]) == 4);
    static_assert(compute_pi(kEpsilonPi[1]) == 4);
    static_assert(compute_pi(kEpsilonPi[2]) == 4 * (1 - 1.0 / 3));
    static_assert(compute_pi(kEpsilonPi[3]) == 4 * (1 - 1.0 / 3 + 1.0 / 5));
    static_assert(compute_pi(kEpsilonPi[4]) == 4 * (1 - 1.0 / 3 + 1.0 / 5 - 1.0 / 7));
    static_assert(compute_pi(kEpsilonPi[5]) == 4 * (1 - 1.0 / 3 + 1.0 / 5 - 1.0 / 7 + 1.0 / 9));

    return 0;
}
