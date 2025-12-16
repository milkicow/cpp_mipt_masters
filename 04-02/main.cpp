#include <concepts>
#include <iostream>
#include <type_traits>

template <std::floating_point T, std::floating_point... Args>
T max(T first, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return first;
    } else {
        return std::max(first, max(args...));
    }
}

template <std::floating_point T, std::floating_point... Args>
T min(T first, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        return first;
    } else {
        return std::min(first, min(args...));
    }
}

template <std::floating_point... Args>
double sum(Args... args) {
    return (args + ...);
}

template <std::floating_point... Args>
double average(Args... args) {
    return sum(args...) / sizeof...(args);
}

int main() {
    std::cout << "max(3.14, 2.71, 1.41) = " << max(3.14, 2.71, 1.41) << '\n';
    std::cout << "min(3.14, 2.71, 1.41) = " << min(3.14, 2.71, 1.41) << '\n';
    std::cout << "sum(1.5, 2.5, 3.5) = " << sum(1.5, 2.5, 3.5) << '\n';
    std::cout << "average(1.0, 2.0, 3.0, 4.0) = " << average(1.0, 2.0, 3.0, 4.0) << '\n';

    return 0;
}
