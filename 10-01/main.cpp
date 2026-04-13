#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void test_vector_growth() {
    std::cout << "\n=== TEST 1: std::vector capacity growth ===\n";
    std::cout << std::left << std::setw(8) << "n" << std::setw(12) << "size" << std::setw(12)
              << "capacity" << std::setw(14) << "ratio (new/old)" << std::setw(20) << "data ptr"
              << "\n";
    std::cout << std::string(66, '-') << "\n";

    std::vector<int> v;
    size_t prev_cap = 0;

    for (int i = 0; i < 1025; ++i) {
        const int* ptr_before = v.data();
        v.push_back(i);
        size_t cap = v.capacity();

        if (cap != prev_cap) {
            double ratio = (prev_cap == 0) ? 0.0 : static_cast<double>(cap) / prev_cap;
            std::cout << std::left << std::setw(8) << (i + 1) << std::setw(12) << v.size()
                      << std::setw(12) << cap << std::setw(14) << std::fixed << std::setprecision(2)
                      << ratio << std::setw(20) << static_cast<const void*>(v.data()) << "\n";
            prev_cap = cap;
        }
        (void)ptr_before;
    }
}

void test_vector_iterator_invalidation() {
    std::cout << "\n=== TEST 2: iterator invalidation on realloc ===\n";

    std::vector<int> v;
    v.reserve(4);
    v.push_back(0x67);

    const int* ptr_first = &v[0];
    std::cout << "ptr before realloc : " << static_cast<const void*>(ptr_first)
              << "  value=" << *ptr_first << "\n";

    while (v.size() < v.capacity()) v.push_back(0);
    v.push_back(0x67);

    std::cout << "ptr after realloc  : " << static_cast<const void*>(v.data()) << "  v[0]=" << v[0]
              << "\n";
    std::cout << "old ptr still holds: " << static_cast<const void*>(ptr_first) << "  dangling\n";
    std::cout << "Pointers equal?    : "
              << (ptr_first == v.data() ? "YES (no realloc)" : "NO  (invalidated)") << "\n";
}

void test_vector_reserve() {
    std::cout << "\n=== TEST 3: reserve() prevents reallocation ===\n";

    std::vector<int> v;
    v.reserve(1000);
    const int* stable_ptr = &v[0];

    for (int i = 0; i < 1000; ++i) v.push_back(i);

    std::cout << "capacity after reserve(1000): " << v.capacity() << "\n";
    std::cout << "size after 1000 push_back   : " << v.size() << "\n";
    std::cout << "data() unchanged?            : " << (v.data() == stable_ptr ? "YES" : "NO")
              << "\n";
}

void test_deque_chunk_size() {
    std::cout << "\n=== TEST 4: std::deque chunk (page) size ===\n";

    std::deque<int> d;
    const size_t N = 512;

    for (size_t i = 0; i < N; ++i) d.push_back(static_cast<int>(i));

    std::cout << std::left << std::setw(8) << "index" << std::setw(20) << "address" << std::setw(14)
              << "delta (bytes)"
              << "block change?\n";
    std::cout << std::string(56, '-') << "\n";

    uintptr_t prev = reinterpret_cast<uintptr_t>(&d[0]);
    int block_idx = 0;

    for (size_t i = 1; i < N; ++i) {
        uintptr_t cur = reinterpret_cast<uintptr_t>(&d[i]);
        ptrdiff_t delta = static_cast<ptrdiff_t>(cur) - static_cast<ptrdiff_t>(prev);
        bool new_block = (delta < 0 || delta > static_cast<ptrdiff_t>(sizeof(int) * 2));

        if (new_block) {
            ++block_idx;
            std::cout << std::left << std::setw(8) << i << std::setw(20)
                      << reinterpret_cast<const void*>(&d[i]) << std::setw(14) << delta
                      << "YES — new block #" << block_idx << "\n";
        }
        prev = cur;
    }

    uintptr_t a0 = reinterpret_cast<uintptr_t>(&d[0]);
    uintptr_t a128 = reinterpret_cast<uintptr_t>(&d[128]);
    std::cout << "\nBlock 0 start: " << reinterpret_cast<const void*>(&d[0]) << "\n";
    std::cout << "Block 1 start: " << reinterpret_cast<const void*>(&d[128]) << "\n";
    std::cout << "Contiguous elements per block : 128\n";
    std::cout << "Page size in bytes            : " << 128 * sizeof(int) << " B\n";
    (void)a0;
    (void)a128;
}

void test_deque_addresses() {
    std::cout << "\n=== TEST 5: deque address layout (push_back vs push_front) ===\n";

    std::deque<int> d;
    const int SHOW = 10;

    std::cout << "--- push_back ---\n";
    std::cout << std::left << std::setw(6) << "n" << std::setw(20) << "address"
              << "delta\n";
    uintptr_t prev = 0;
    for (int i = 0; i < SHOW; ++i) {
        d.push_back(i);
        uintptr_t cur = reinterpret_cast<uintptr_t>(&d.back());
        std::ostringstream oss;
        if (prev) oss << "+" << (static_cast<ptrdiff_t>(cur) - static_cast<ptrdiff_t>(prev));
        std::cout << std::left << std::setw(6) << (i + 1) << std::setw(20)
                  << reinterpret_cast<const void*>(&d.back()) << oss.str() << "\n";
        prev = cur;
    }

    std::deque<int> d2;
    std::cout << "--- push_front ---\n";
    std::cout << std::left << std::setw(6) << "n" << std::setw(20) << "address"
              << "delta\n";
    prev = 0;
    for (int i = 0; i < SHOW; ++i) {
        d2.push_front(i);
        uintptr_t cur = reinterpret_cast<uintptr_t>(&d2.front());
        std::ostringstream oss;
        if (prev) oss << (static_cast<ptrdiff_t>(cur) - static_cast<ptrdiff_t>(prev));
        std::cout << std::left << std::setw(6) << (i + 1) << std::setw(20)
                  << reinterpret_cast<const void*>(&d2.front()) << oss.str() << "\n";
        prev = cur;
    }
}

void test_deque_pointer_stability() {
    std::cout << "\n=== TEST 6: deque pointer stability ===\n";

    std::deque<int> d;
    d.push_back(100);
    const int* ptr_middle = &d[0];

    for (int i = 0; i < 200; ++i) {
        if (i % 2 == 0)
            d.push_back(i);
        else
            d.push_front(i);
    }

    std::cout << "ptr to d[0] saved before 200 insertions\n";
    std::cout << "Value at saved ptr: " << *ptr_middle << "\n";
    size_t idx = 0;
    for (size_t i = 0; i < d.size(); ++i) {
        if (&d[i] == ptr_middle) {
            idx = i;
            break;
        }
    }
    std::cout << "Element found at index: " << idx << "  d[" << idx << "]=" << d[idx] << "\n";
    std::cout << "Pointer still valid?  : " << (d[idx] == 100 ? "YES" : "NO") << "\n";
}

int main() {
    test_vector_growth();
    test_vector_iterator_invalidation();
    test_vector_reserve();
    test_deque_chunk_size();
    test_deque_addresses();
    test_deque_pointer_stability();

    std::cout << "\n=== ALL TESTS DONE ===\n";
    return 0;
}
