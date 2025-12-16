#ifndef LCM
#define LCM

#include <utility>

#include "gcd.hpp"

namespace lcm {

int lcm_iterative(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    auto a_abs = std::abs(a);
    auto b_abs = std::abs(b);

    return (a_abs / gcd::gcd_iterative(a_abs, b_abs)) * b_abs;
}

int lcm_recursive(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    auto a_abs = std::abs(a);
    auto b_abs = std::abs(b);

    return (a_abs / gcd::gcd_recursive(a_abs, b_abs)) * b_abs;
}

}  // namespace lcm

#endif  // LCM