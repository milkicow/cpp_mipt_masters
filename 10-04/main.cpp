#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <cassert>
#include <iomanip>
#include <iostream>

namespace ublas = boost::numeric::ublas;

using ull = unsigned long long int;
using Matrix = ublas::matrix<ull>;

Matrix multiply(const Matrix& A, const Matrix& B) {
    Matrix C(2, 2, 0);

    for (std::size_t i = 0; i < 2; ++i)
        for (std::size_t k = 0; k < 2; ++k)
            for (std::size_t j = 0; j < 2; ++j) C(i, j) += A(i, k) * B(k, j);

    return C;
}

Matrix matrix_power(Matrix B, ull n) {
    Matrix R(2, 2, 0);
    R(0, 0) = R(1, 1) = 1;

    while (n > 0) {
        if (n & 1) R = multiply(R, B);

        B = multiply(B, B);
        n >>= 1;
    }

    return R;
}

ull fibonacci(ull n) {
    if (n == 0) return 0;

    Matrix Q(2, 2);
    Q(0, 0) = 1;
    Q(0, 1) = 1;
    Q(1, 0) = 1;
    Q(1, 1) = 0;

    Matrix R = matrix_power(Q, n);
    return R(0, 1);
}

int main() {
    const ull known[] = {0,  1,  1,   2,   3,   5,   8,   13,   21,   34,
                         55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181};

    std::cout << "Verification:\n";
    std::cout << std::string(32, '-') << "\n";
    std::cout << std::setw(6) << "n" << std::setw(12) << "F(n)" << std::setw(8) << "OK?\n";
    std::cout << std::string(32, '-') << "\n";

    for (ull i = 0; i < 20; ++i) {
        ull result = fibonacci(i);
        bool ok = (result == known[i]);

        std::cout << std::setw(6) << i << std::setw(12) << result << std::setw(8)
                  << (ok ? "YES" : "NO!") << "\n";

        assert(ok);
    }

    std::cout << "\nLarge values:\n";
    std::cout << std::string(32, '-') << "\n";

    for (ull n : {30ULL, 50ULL, 70ULL, 80ULL, 85ULL, 90ULL, 93ULL})
        std::cout << "F(" << n << ") = " << fibonacci(n) << "\n";

    return 0;
}