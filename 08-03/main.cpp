#include <cassert>
#include <limits>

auto ilog2(int n) -> int {
    auto bits = static_cast<unsigned int>(n);

    int result = 0;

    while (bits >>= 1) {
        ++result;
    }

    return result;
}

auto ilog2(float x) -> int {
    union {
        float f;
        unsigned int u;
    } cast;

    cast.f = x;

    unsigned int exponent = (cast.u >> 23) & 0xFFu;

    if (exponent == 0xFFu) {
        return std::numeric_limits<int>::max();
    }

    if (exponent != 0) {
        return static_cast<int>(exponent) - 127;
    } else {
        unsigned int mantissa = cast.u & 0x7FFFFFu;

        int result = -126;

        while (!(mantissa & 0x400000u)) {
            mantissa <<= 1;

            --result;
        }

        return result - 1;
    }
}

int main() {
    // ilog2(int): floor of log2 for positive integers

    assert(ilog2(1) == 0);
    assert(ilog2(2) == 1);
    assert(ilog2(3) == 1);
    assert(ilog2(4) == 2);
    assert(ilog2(7) == 2);
    assert(ilog2(8) == 3);
    assert(ilog2(255) == 7);
    assert(ilog2(256) == 8);
    assert(ilog2(1024) == 10);

    //  ----------------------------------------------------------------------------------------

    // ilog2(float): normalized numbers

    assert(ilog2(1.0f) == 0);
    assert(ilog2(2.0f) == 1);
    assert(ilog2(4.0f) == 2);
    assert(ilog2(0.5f) == -1);
    assert(ilog2(0.25f) == -2);
    assert(ilog2(1.5f) == 0);
    assert(ilog2(3.0f) == 1);
    assert(ilog2(8.0f) == 3);

    //  ----------------------------------------------------------------------------------------

    // ilog2(float): denormalized numbers (exponent = 0, no implicit leading 1)
    // smallest positive normal float is 2^-126; denormals are below that

    float smallest_normal = std::numeric_limits<float>::min();            // 2^-126
    float smallest_subnormal = std::numeric_limits<float>::denorm_min();  // 2^-149

    assert(ilog2(smallest_normal) == -126);
    assert(ilog2(smallest_subnormal) == -149);

    //  ----------------------------------------------------------------------------------------

    // ilog2(float): inf and nan return INT_MAX sentinel

    assert(ilog2(std::numeric_limits<float>::infinity()) == std::numeric_limits<int>::max());
    assert(ilog2(std::numeric_limits<float>::quiet_NaN()) == std::numeric_limits<int>::max());
    assert(ilog2(std::numeric_limits<float>::signaling_NaN()) == std::numeric_limits<int>::max());

    return 0;
}
