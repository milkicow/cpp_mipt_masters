#ifndef GCD
#define GCD

#include <cassert>
#include <cstdlib>
#include <utility>

namespace gcd {

int remainder(int a, int b) {
    if (int r = a % b; r >= 0) {
        return r;
    } else {
        return r + std::abs(b);
    }
}

int gcd_iterative(int a, int b) {
    int r = remainder(a, b);
    while (r != 0) {
        a = std::exchange(b, r);
        r = remainder(a, b);
    }
    return std::abs(b);
}

int gcd_recursive(int a, int b) {
    int r = remainder(a, b);
    return r != 0 ? gcd_recursive(b, r) : std::abs(b);
}
}  // namespace gcd

#endif  // GCD