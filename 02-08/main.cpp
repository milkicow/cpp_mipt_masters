#include <iostream>
#include <vector>

class Collatz {
   private:
    using num_t = unsigned long long;
    using len_t = std::size_t;

    using cache_t = std::vector<len_t>;

    cache_t cache;

   public:
    Collatz(std::size_t cache_size = 10000) : cache(cache_size, 0) {}

    len_t seq_len(num_t number) {
        if (number == 0) {
            throw std::runtime_error{"only positive numbers could be accepted"};
        }

        auto next_number = [](num_t num) {
            return (num & num_t{1}) ? num_t{3} * num + num_t{1} : num / num_t{2};
        };

        len_t seq_len = 0;
        for (auto n = number; n != 1; n = next_number(n), ++seq_len) {
            if (auto n_num_seq_len = cache[n]; n_num_seq_len) {
                seq_len += n_num_seq_len;
                break;
            }
        }

        cache[number] = seq_len;
        return seq_len;
    }

    len_t get_max_len() { return *std::max_element(cache.begin(), cache.end()); }
};

int main() {
    const std::size_t kMaxSeq = 100;

    Collatz collatz{};
    for (std::size_t num = 1; num != kMaxSeq; ++num) {
        auto seq_len = collatz.seq_len(num);
        std::cout << seq_len << '\n';
    }

    std::cout << "max seq len: " << collatz.get_max_len() << '\n';

    return 0;
}