#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main() {
    const std::string target = "methinksitislikeaweasel";

    const std::size_t length = target.size();
    const std::size_t copies = 100;
    const double mutation = 0.05;
    const char first_char = 'a';
    const char last_char = 'z';

    std::random_device device;

    std::default_random_engine engine(device());

    std::uniform_int_distribution<int> char_dist(first_char, last_char);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    std::string current(length, ' ');

    for (auto& c : current) {
        c = static_cast<char>(char_dist(engine));
    }

    auto distance = [&](const std::string& s) -> int {
        int result = 0;

        for (auto i = 0uz; i < length; ++i) {
            if (s[i] != target[i]) {
                ++result;
            }
        }

        return result;
    };

    int iteration = 0;

    while (true) {
        std::cout << "iteration " << iteration << " : " << current << "\n";
        std::vector<std::string> pool(copies);

        for (auto& child : pool) {
            child = current;

            for (auto& c : child) {
                if (prob_dist(engine) < mutation) {
                    c = static_cast<char>(char_dist(engine));
                }
            }
        }

        auto best = std::min_element(
            pool.begin(), pool.end(),
            [&](const std::string& a, const std::string& b) { return distance(a) < distance(b); });

        if (distance(*best) == 0) {
            std::cout << "iteration " << iteration + 1 << " : " << *best << "\n";
            break;
        }
        current = *best;
        ++iteration;
    }

    return 0;
}
