#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std::literals;

unsigned int RSHash(const char* str, unsigned int length) {
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash = hash * a + (*str);
        a = a * b;
    }

    return hash;
}

unsigned int JSHash(const char* str, unsigned int length) {
    unsigned int hash = 1315423911;

    for (unsigned int i = 0; i < length; ++str, ++i) hash ^= ((hash << 5) + (*str) + (hash >> 2));

    return hash;
}

unsigned int PJWHash(const char* str, unsigned int length) {
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters = (unsigned int)((BitsInUnsignedInt * 3) / 4);
    const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);

    unsigned int hash = 0;
    unsigned int test = 0;

    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0) hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }

    return hash;
}

unsigned int ELFHash(const char* str, unsigned int length) {
    unsigned int hash = 0;
    unsigned int x = 0;

    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash = (hash << 4) + (*str);

        if ((x = hash & 0xF0000000L) != 0) hash ^= (x >> 24);

        hash &= ~x;
    }

    return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length) {
    unsigned int seed = 131;
    unsigned int hash = 0;

    for (unsigned int i = 0; i < length; ++str, ++i) hash = (hash * seed) + (*str);

    return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length) {
    unsigned int hash = 0;

    for (unsigned int i = 0; i < length; ++str, ++i)
        hash = (*str) + (hash << 6) + (hash << 16) - hash;

    return hash;
}

unsigned int DJBHash(const char* str, unsigned int length) {
    unsigned int hash = 5381;

    for (unsigned int i = 0; i < length; ++str, ++i) hash = ((hash << 5) + hash) + (*str);

    return hash;
}

unsigned int DEKHash(const char* str, unsigned int length) {
    unsigned int hash = length;

    for (unsigned int i = 0; i < length; ++str, ++i) hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);

    return hash;
}

unsigned int APHash(const char* str, unsigned int length) {
    unsigned int hash = 0xAAAAAAAA;

    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3))
                               : ~((hash << 11) + ((*str) ^ (hash >> 5)));
    }

    return hash;
}

std::vector<std::string> generate_strings(std::size_t count) {
    std::vector<std::string> result;
    result.reserve(count);

    uint32_t rng = 0xDEADBEEF;

    auto next_char = [&]() -> char {
        rng = rng * 1664525u + 1013904223u;
        return static_cast<char>('a' + (rng >> 24) % 26);
    };

    for (std::size_t i = 0; i < count; ++i) {
        rng = rng * 1664525u + 1013904223u;
        std::size_t len = 4 + (rng >> 24) % 16;

        std::string s;
        s.reserve(len);

        for (std::size_t c = 0; c < len; ++c) s += next_char();

        result.push_back(std::move(s));
    }

    return result;
}

using HashFn = unsigned int (*)(const char*, unsigned int);

std::size_t count_collisions(const std::vector<std::string>& strings, HashFn fn) {
    std::unordered_map<unsigned int, std::size_t> freq;
    freq.reserve(strings.size() * 2);

    for (const auto& s : strings) ++freq[fn(s.c_str(), static_cast<unsigned int>(s.size()))];

    std::size_t collisions = 0;

    for (const auto& [hash, cnt] : freq)
        if (cnt > 1) collisions += cnt - 1;

    return collisions;
}

int main() {
    struct Fn {
        const char* name;
        HashFn fn;
    };

    const Fn fns[] = {
        {"RS", RSHash},   {"JS", JSHash},     {"PJW", PJWHash},
        {"ELF", ELFHash}, {"BKDR", BKDRHash}, {"SDBM", SDBMHash},
        {"DJB", DJBHash}, {"DEK", DEKHash},   {"AP", APHash},
    };

    const std::size_t steps[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
    const std::size_t max_count = steps[std::size(steps) - 1];

    const auto pool = generate_strings(max_count);

    std::cout << std::setw(8) << "N";
    for (const auto& f : fns) std::cout << std::setw(10) << f.name;
    std::cout << "\n" << std::string(8 + 9 * 10, '-') << "\n";

    std::ofstream csv("collisions.csv");
    csv << "N";

    for (const auto& f : fns) csv << "," << f.name;

    csv << "\n";

    for (std::size_t n : steps) {
        std::vector<std::string> subset(pool.begin(), pool.begin() + n);

        std::cout << std::setw(8) << n;

        csv << n;

        for (const auto& f : fns) {
            std::size_t c = count_collisions(subset, f.fn);
            std::cout << std::setw(10) << c;
            csv << "," << c;
        }
        csv << "\n";

        std::cout << "\n";
    }

    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "Ranking at N = 100 000 (fewer collisions = better)\n";
    std::cout << std::string(50, '=') << "\n";

    std::vector<std::pair<std::size_t, std::string>> ranking;

    for (const auto& f : fns) {
        std::size_t c = count_collisions(pool, f.fn);
        ranking.emplace_back(c, f.name);
    }

    std::sort(ranking.begin(), ranking.end());

    int rank = 1;
    for (const auto& [c, name] : ranking)
        std::cout << std::setw(3) << rank++ << ". " << name << "  collisions = " << c << "\n";

    return 0;
}
