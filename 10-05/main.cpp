#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::literals;

unsigned int RSHash(const char* str, unsigned int length) {
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash = hash * a + (*str);
        a *= b;
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
    unsigned int hash = 0, test = 0;
    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash = (hash << OneEighth) + (*str);
        if ((test = hash & HighBits) != 0) hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
    return hash;
}

unsigned int ELFHash(const char* str, unsigned int length) {
    unsigned int hash = 0, x = 0;
    for (unsigned int i = 0; i < length; ++str, ++i) {
        hash = (hash << 4) + (*str);
        if ((x = hash & 0xF0000000L) != 0) hash ^= (x >> 24);
        hash &= ~x;
    }
    return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length) {
    unsigned int hash = 0;
    for (unsigned int i = 0; i < length; ++str, ++i) hash = hash * 131 + (*str);
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
    std::mt19937 rng(0xDEADBEEF);
    std::uniform_int_distribution<> len_dist(4, 19);
    std::uniform_int_distribution<> chr_dist(0, 25);

    std::vector<std::string> result;
    result.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        std::string s;
        std::size_t len = static_cast<std::size_t>(len_dist(rng));
        s.reserve(len);
        for (std::size_t c = 0; c < len; ++c) s += static_cast<char>('a' + chr_dist(rng));
        result.push_back(std::move(s));
    }
    return result;
}

using HashFn = unsigned int (*)(const char*, unsigned int);

struct Fn {
    const char* name;
    HashFn fn;
};

int main() {
    const Fn fns[] = {
        {"RS", RSHash},   {"JS", JSHash},     {"PJW", PJWHash},
        {"ELF", ELFHash}, {"BKDR", BKDRHash}, {"SDBM", SDBMHash},
        {"DJB", DJBHash}, {"DEK", DEKHash},   {"AP", APHash},
    };
    const std::size_t NFN = std::size(fns);

    const std::size_t steps[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
    const std::size_t max_n = steps[std::size(steps) - 1];

    const auto pool = generate_strings(max_n);

    std::vector<std::vector<unsigned int>> hashes(NFN, std::vector<unsigned int>(max_n));
    for (std::size_t fi = 0; fi < NFN; ++fi)
        for (std::size_t i = 0; i < max_n; ++i)
            hashes[fi][i] = fns[fi].fn(pool[i].c_str(), static_cast<unsigned int>(pool[i].size()));

    auto count_collisions = [&](std::size_t fi, std::size_t n) -> std::size_t {
        std::unordered_map<unsigned int, std::size_t> freq;
        freq.reserve(n * 2);
        for (std::size_t i = 0; i < n; ++i) ++freq[hashes[fi][i]];
        std::size_t c = 0;
        for (const auto& [h, cnt] : freq)
            if (cnt > 1) c += cnt - 1;
        return c;
    };

    std::cout << std::setw(8) << "N";
    for (const auto& f : fns) std::cout << std::setw(10) << f.name;
    std::cout << "\n" << std::string(8 + NFN * 10, '-') << "\n";

    std::ofstream csv("collisions.csv");
    csv << "N";
    for (const auto& f : fns) csv << "," << f.name;
    csv << "\n";

    for (std::size_t n : steps) {
        std::cout << std::setw(8) << n;
        csv << n;
        for (std::size_t fi = 0; fi < NFN; ++fi) {
            std::size_t c = count_collisions(fi, n);
            std::cout << std::setw(10) << c;
            csv << "," << c;
        }
        std::cout << "\n";
        csv << "\n";
    }

    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "Ranking at N = " << max_n << " (fewer collisions = better)\n";
    std::cout << std::string(50, '=') << "\n";

    std::vector<std::pair<std::size_t, std::string>> ranking;
    for (std::size_t fi = 0; fi < NFN; ++fi)
        ranking.emplace_back(count_collisions(fi, max_n), fns[fi].name);
    std::sort(ranking.begin(), ranking.end());

    int rank = 1;
    for (const auto& [c, name] : ranking)
        std::cout << std::setw(3) << rank++ << ". " << std::setw(5) << name
                  << "  collisions = " << c << "\n";

    return 0;
}