#include <cmath>
#include <exception>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

using Roots = std::variant<std::monostate, double, std::pair<double, double>>;

using MaybeRoots = std::optional<Roots>;

static bool is_zero(double num) {
    const double epsilon = 1e-6;
    return std::abs(num) < epsilon;
}

static double get_coeff(std::string_view coeff_name) {
    double coeff;
    std::cout << "enter " << coeff_name << '\n';
    std::cin >> coeff;
    return coeff;
}

static MaybeRoots solve(double a, double b, double c) {
    if (is_zero(a)) {
        if (is_zero(b)) {
            if (is_zero(c)) {
                return Roots{std::monostate{}};
            } else {
                return std::nullopt;
            }
        } else {
            return Roots{-c / b};
        }
    } else {
        const double discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0) {
            return std::nullopt;
        }
        const double sq = std::sqrt(discriminant);
        const double two_a = 2.0 * a;
        if (is_zero(discriminant)) {
            return Roots{-b / two_a};
        } else {
            return Roots{std::make_pair((-b - sq) / two_a, (-b + sq) / two_a)};
        }
    }
}

int main() try {
    const auto a = get_coeff("a");
    const auto b = get_coeff("b");
    const auto c = get_coeff("c");

    const MaybeRoots result = solve(a, b, c);

    if (!result.has_value()) {
        std::cout << "no roots\n";
    } else if (std::holds_alternative<std::monostate>(*result)) {
        std::cout << "root is any number\n";
    } else if (std::holds_alternative<double>(*result)) {
        std::cout << "one root: " << std::get<double>(*result) << '\n';
    } else {
        const auto [x1, x2] = std::get<std::pair<double, double>>(*result);
        std::cout << "two roots: " << x1 << ' ' << x2 << '\n';
    }

    return 0;
} catch (const std::exception& e) {
    std::cerr << "Caught exception: " << e.what() << '\n';
    return EXIT_FAILURE;
}
