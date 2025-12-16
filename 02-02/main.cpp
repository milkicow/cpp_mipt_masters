#include <exception>
#include <iostream>
#include <utility>

static bool is_zero(double num) {
    const double epsilon = 10e-6;
    return std::abs(num) < epsilon;
}

static double get_coeff(std::string_view coeff_name) {
    double coeff;

    std::cout << "enter " << coeff_name << '\n';
    std::cin >> coeff;

    return coeff;
}

static double solve_linear_equation(double b, double c) { return -c / b; }

static std::pair<double, double> solve_quadratic_equation(double a, double b, double c) {
    const auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        throw std::runtime_error("No real roots: discriminant is negative");
    }
    const auto sq_root_of_d = std::sqrt(discriminant);
    const auto twice_a = 2 * a;
    return {(-b + sq_root_of_d) / twice_a, (-b + sq_root_of_d) / twice_a};
}

int main() try {
    const auto a = get_coeff("a");
    const auto b = get_coeff("b");
    const auto c = get_coeff("c");

    if (is_zero(a)) {
        if (is_zero(b)) {
            if (is_zero(c)) {
                std::cout << "root is any number\n";
            } else {
                std::cout << "no roots\n";
            }
        } else {
            auto x = solve_linear_equation(b, c);
            std::cout << "one root: " << x << '\n';
        }
    } else {
        auto [x1, x2] = solve_quadratic_equation(a, b, c);
        std::cout << "two roots: " << x1 << ' ' << x2 << '\n';
    }

    return 0;
} catch (const std::exception &e) {
    std::cerr << "Caught exception: " << e.what() << '\n';
    return EXIT_FAILURE;
}