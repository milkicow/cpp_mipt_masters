#include <array>
#include <cmath>
#include <iostream>

int main() {
    const std::size_t kNumbersSize = 100;

    int n;
    std::cin >> n;

    if (n <= 0) {
        std::cout << "invalid number of elements\n";
        return EXIT_FAILURE;
    }

    if (n > kNumbersSize) {
        std::cout << "number of elements exceeded kNumbersSize\n";
        return EXIT_FAILURE;
    }

    std::array<double, kNumbersSize> numbers;

    for (int i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    double min_val = numbers[0];
    double max_val = numbers[0];
    double sum = 0.0;

    for (const double num : numbers) {
        if (num < min_val) min_val = num;
        if (num > max_val) max_val = num;
        sum += num;
    }

    double mean = sum / n;

    double variance = 0.0;
    for (const double num : numbers) {
        variance += (num - mean) * (num - mean);
    }
    double std_dev = std::sqrt(variance / n);

    std::cout << "min: " << min_val << "\n";
    std::cout << "max: " << max_val << "\n";
    std::cout << "mean: " << mean << "\n";
    std::cout << "standard deviation: " << std_dev << "\n";

    return 0;
}