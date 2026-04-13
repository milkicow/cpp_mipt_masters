#include <benchmark/benchmark.h>

#include <boost/noncopyable.hpp>
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <new>
#include <print>
#include <random>
#include <utility>
#include <vector>

class Allocator : private boost::noncopyable {
   public:
    enum class Policy { FirstFit, BestFit };

    Allocator(std::size_t size, Policy policy = Policy::FirstFit) : m_size(size), m_policy(policy) {
        assert(m_size >= sizeof(Node) + 1);

        m_begin = operator new(m_size, std::align_val_t(s_alignment));

        m_head = get_node(m_begin);

        m_head->size = m_size - sizeof(Header);

        m_head->next = nullptr;
    }

    ~Allocator() { operator delete(m_begin, m_size, std::align_val_t(s_alignment)); }

    auto allocate(std::size_t size) -> void * {
        void *end = get_byte(m_begin) + sizeof(Header) + size, *next = end;

        auto free = 2 * alignof(Header);

        if (next = std::align(alignof(Header), sizeof(Header), next, free); next) {
            auto padding = get_byte(next) - get_byte(end);

            auto [current, previous] = (m_policy == Policy::FirstFit) ? find_first(size + padding)
                                                                      : find_best(size + padding);

            if (current) {
                if (current->size >= size + padding + sizeof(Node) + 1) {
                    auto step = sizeof(Header) + size + padding;

                    auto node = get_node(get_byte(current) + step);

                    node->size = current->size - step;

                    node->next = current->next;

                    current->next = node;
                } else {
                    padding += current->size - size - padding;
                }

                if (!previous) {
                    m_head = current->next;
                } else {
                    previous->next = current->next;
                }

                auto header = get_header(current);

                header->size = size + padding;

                return get_byte(current) + sizeof(Header);
            }
        }

        return nullptr;
    }

    void deallocate(void *x) {
        auto node = get_node(get_byte(x) - sizeof(Header));

        Node *previous = nullptr, *current = m_head;

        while (current) {
            if (node < current) {
                node->next = current;

                if (!previous) {
                    m_head = node;
                } else {
                    previous->next = node;
                }

                break;
            }

            previous = current;

            current = current->next;
        }

        merge(previous, node);
    }

    void show() const {
        std::print("Allocator::show : m_size = {} m_begin = {:018} m_head = {:018} ",

                   m_size, m_begin, static_cast<void *>(m_head));

        if (m_head && m_head->next) {
            std::print("m_head->next = {:018}\n", static_cast<void *>(m_head->next));
        } else {
            std::print("\n");
        }
    }

   private:
    struct Node {
        std::size_t size = 0;

        Node *next = nullptr;
    };

    struct alignas(std::max_align_t) Header {
        std::size_t size = 0;
    };

    auto get_byte(void *x) const -> std::byte * { return static_cast<std::byte *>(x); }

    auto get_node(void *x) const -> Node * { return static_cast<Node *>(x); }

    auto get_header(void *x) const -> Header * { return static_cast<Header *>(x); }

    auto find_first(std::size_t size) const -> std::pair<Node *, Node *> {
        Node *current = m_head, *previous = nullptr;

        while (current && size > current->size) {
            previous = current;

            current = current->next;
        }

        return std::make_pair(current, previous);
    }


    auto find_best(std::size_t size) const -> std::pair<Node *, Node *> {
        Node *best = nullptr;
        Node *best_previous = nullptr;

        Node *current = m_head;
        Node *previous = nullptr;

        while (current) {
            if (current->size >= size) {
                if (!best || current->size < best->size) {
                    best = current;
                    best_previous = previous;
                }
            }

            previous = current;
            current = current->next;
        }

        return std::make_pair(best, best_previous);
    }

    void merge(Node *previous, Node *node) const {
        if (node->next && get_byte(node) + sizeof(Header) + node->size == get_byte(node->next)) {
            node->size += sizeof(Header) + node->next->size;

            node->next = node->next->next;
        }

        if (previous && get_byte(previous) + sizeof(Header) + previous->size == get_byte(node)) {
            previous->size += sizeof(Header) + node->size;

            previous->next = node->next;
        }
    }

    std::size_t m_size = 0;
    Policy m_policy = Policy::FirstFit;

    void *m_begin = nullptr;

    Node *m_head = nullptr;

    static inline auto s_alignment = alignof(std::max_align_t);
};


template <Allocator::Policy P>
void run_benchmark(benchmark::State &state) {
    auto kb = 1uz << 10, mb = 1uz << 20, gb = 1uz << 30;

    std::uniform_int_distribution distribution(1, 16);

    std::default_random_engine engine;

    std::vector<void *> vector(kb, nullptr);

    for (auto element : state) {
        Allocator allocator(16 * gb, P);

        for (auto i = 0uz; i < kb; ++i) {
            vector[i] = allocator.allocate(distribution(engine) * mb);
        }

        for (auto i = 0uz; i < kb; i += 32) {
            allocator.deallocate(vector[i]);
        }

        for (auto i = 0uz; i < kb; i += 32) {
            vector[i] = allocator.allocate(distribution(engine) * mb);
        }

        for (auto i = 0uz; i < kb; ++i) {
            allocator.deallocate(vector[i]);
        }

        benchmark::DoNotOptimize(vector);
    }
}

void test_first_fit(benchmark::State &state) { run_benchmark<Allocator::Policy::FirstFit>(state); }
void test_best_fit(benchmark::State &state) { run_benchmark<Allocator::Policy::BestFit>(state); }

BENCHMARK(test_first_fit);
BENCHMARK(test_best_fit);

int main() {
    for (auto policy : {Allocator::Policy::FirstFit, Allocator::Policy::BestFit}) {
        Allocator allocator(1 << 10, policy);

        std::print("\n--- {} ---\n",
                   policy == Allocator::Policy::FirstFit ? "First Fit" : "Best Fit");

        allocator.show();
        allocator.allocate(16);

        allocator.show();
        auto x = allocator.allocate(16);

        allocator.show();
        auto y = allocator.allocate(16);

        allocator.show();
        allocator.allocate(16);

        allocator.show();
        allocator.deallocate(y);

        allocator.show();
        allocator.deallocate(x);

        allocator.show();
        auto z = allocator.allocate(32);

        allocator.show();

        assert(z == x);
    }
    benchmark::RunSpecifiedBenchmarks();
}
