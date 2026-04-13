#include <benchmark/benchmark.h>

#include <boost/noncopyable.hpp>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <new>
#include <print>
#include <random>
#include <utility>
#include <vector>

class Allocator : private boost::noncopyable {
   public:
    explicit Allocator(std::size_t size) : m_size(size) {
        m_begin = operator new(m_size, std::align_val_t(s_alignment));
    }

    virtual ~Allocator() { operator delete(m_begin, m_size, std::align_val_t(s_alignment)); }

    [[nodiscard]] virtual auto allocate(std::size_t size, std::size_t alignment = s_alignment)
        -> void * = 0;

    virtual void deallocate(void *x) = 0;

    virtual void show() const = 0;

    static inline auto s_alignment = alignof(std::max_align_t);

   protected:
    struct Header {
        std::size_t prev_offset = 0;
    };

    struct Node {
        std::size_t size = 0;
        Node *next = nullptr;
    };

    template <typename T>
        requires(std::is_same_v<T, std::byte> || std::is_same_v<T, Header> ||
                 std::is_same_v<T, Node>)
    static auto get(void *ptr) noexcept -> T * {
        return static_cast<T *>(ptr);
    }

    std::size_t m_size = 0;
    void *m_begin = nullptr;
};

class LinearAllocator final : public Allocator {
   public:
    explicit LinearAllocator(std::size_t size) : Allocator(size) {}

    auto allocate(std::size_t size, std::size_t alignment = s_alignment) -> void * override {
        void *begin = get<std::byte>(m_begin) + m_offset;

        auto free = m_size - m_offset;

        if (begin = std::align(alignment, size, begin, free); begin) {
            m_offset = m_size - free + size;

            return begin;
        } else {
            return nullptr;
        }
    }

    void deallocate(void * /*x*/) override {}

    void show() const override {
        std::print("LinearAllocator::show : m_size = {} m_begin = {:018} m_offset = {:0>4}\n",
                   m_size, m_begin, m_offset);
    }

   private:
    std::size_t m_offset = 0;
};

class StackAllocator final : public Allocator {
   public:
    explicit StackAllocator(std::size_t size) : Allocator(size) {}

    auto allocate(std::size_t size, std::size_t alignment = s_alignment) -> void * override {
        void *begin = get<std::byte>(m_begin) + m_offset + sizeof(header_t);

        auto free = m_size - m_offset - sizeof(header_t);

        if (begin = std::align(alignment, size, begin, free); begin) {
            auto header = get_header(get<std::byte>(begin) - sizeof(header_t));

            *header = std::distance(get<std::byte>(m_begin) + m_offset, get<std::byte>(begin));

            m_offset = get<std::byte>(begin) - get<std::byte>(m_begin) + size;

            return begin;
        } else {
            return nullptr;
        }
    }

    void deallocate(void *x) override {
        auto header = get_header(get<std::byte>(x) - sizeof(header_t));

        m_offset = get<std::byte>(x) - get<std::byte>(m_begin) - *header;
    }

    void show() const override {
        std::print("StackAllocator::show : m_size = {} m_begin = {:018} m_offset = {:0>4}\n",
                   m_size, m_begin, m_offset);
    }

   private:
    using header_t = std::uint8_t;

    auto get_header(void *x) const -> header_t * { return static_cast<header_t *>(x); }

    std::size_t m_offset = 0;
};

class ListAllocator final : public Allocator {
   public:
    ListAllocator(std::size_t size, std::size_t step) : Allocator(size), m_step(step) {
        assert(m_size % m_step == 0 && m_step >= sizeof(Node));
        make_list();
    }

    ~ListAllocator() override {
        for (auto list : m_lists) {
            operator delete(list, m_size, std::align_val_t(s_alignment));
        }
    }

    auto allocate(std::size_t /*size*/ = 0, std::size_t /*alignment*/ = s_alignment)
        -> void * override {
        if (!m_head) {
            if (m_offset == std::size(m_lists)) {
                make_list();
            } else {
                m_head = get<Node>(m_lists[++m_offset - 1]);
            }
        }

        auto node = m_head;

        if (!node->next) {
            auto next = get<std::byte>(node) + m_step;

            if (next != get<std::byte>(m_lists[m_offset - 1]) + m_size) {
                m_head = get<Node>(next);
                m_head->next = nullptr;
            } else {
                m_head = m_head->next;
            }
        } else {
            m_head = m_head->next;
        }

        return node;
    }

    void deallocate(void *x) override {
        if (!x) return;

        auto node = get<Node>(x);
        node->next = m_head;
        m_head = node;
    }

    void show() const override {
        std::print("ListAllocator::show : ");
        std::print("m_size = {} m_step = {} m_begin = {:018} m_head = {:018} m_offset = {}\n",
                   m_size, m_step, m_begin, static_cast<void *>(m_head), m_offset);
    }

   private:
    void make_list() {
        void *slab = operator new(m_size, std::align_val_t(s_alignment));

        m_head = get<Node>(slab);
        m_head->next = nullptr;

        ++m_offset;

        m_lists.push_back(slab);
    }

    std::size_t m_step = 0;
    std::size_t m_offset = 0;
    Node *m_head = nullptr;
    std::vector<void *> m_lists;
};

class FreelistAllocator final : public Allocator {
   public:
    explicit FreelistAllocator(std::size_t size) : Allocator(size) {
        assert(m_size >= sizeof(Node) + 1);

        m_head = get<Node>(m_begin);
        m_head->size = m_size - sizeof(FLHeader);
        m_head->next = nullptr;
    }

    auto allocate(std::size_t size, std::size_t /*alignment*/ = s_alignment) -> void * override {
        void *end = get<std::byte>(m_begin) + sizeof(FLHeader) + size;
        void *next = end;

        auto free = 2 * alignof(FLHeader);

        if (next = std::align(alignof(FLHeader), sizeof(FLHeader), next, free); next) {
            auto padding = get<std::byte>(next) - get<std::byte>(end);

            if (auto [current, previous] = find(size + padding); current) {
                if (current->size >= size + padding + sizeof(Node) + 1) {
                    auto step = sizeof(FLHeader) + size + padding;

                    auto node = get<Node>(get<std::byte>(current) + step);

                    node->size = current->size - step;
                    node->next = current->next;
                    current->next = node;
                } else {
                    padding += current->size - size - padding;
                }

                if (!previous)
                    m_head = current->next;
                else
                    previous->next = current->next;

                auto header = get_flheader(current);
                header->size = size + padding;

                return get<std::byte>(current) + sizeof(FLHeader);
            }
        }

        return nullptr;
    }

    void deallocate(void *x) override {
        auto node = get<Node>(get<std::byte>(x) - sizeof(FLHeader));

        Node *previous = nullptr;
        Node *current = m_head;

        while (current) {
            if (node < current) {
                node->next = current;

                if (!previous)
                    m_head = node;
                else
                    previous->next = node;

                break;
            }

            previous = current;
            current = current->next;
        }

        merge(previous, node);
    }

    void show() const override {
        std::print("FreelistAllocator::show : m_size = {} m_begin = {:018} m_head = {:018} ",
                   m_size, m_begin, static_cast<void *>(m_head));

        if (m_head->next)
            std::print("m_head->next = {:018}\n", static_cast<void *>(m_head->next));
        else
            std::print("\n");
    }

   private:
    struct alignas(std::max_align_t) FLHeader {
        std::size_t size = 0;
    };

    auto get_flheader(void *x) const -> FLHeader * { return static_cast<FLHeader *>(x); }

    auto find(std::size_t size) const -> std::pair<Node *, Node *> {
        Node *current = m_head;
        Node *previous = nullptr;

        while (current && size > current->size) {
            previous = current;
            current = current->next;
        }

        return std::make_pair(current, previous);
    }

    void merge(Node *previous, Node *node) const {
        if (node->next &&
            get<std::byte>(node) + sizeof(FLHeader) + node->size == get<std::byte>(node->next)) {
            node->size += sizeof(FLHeader) + node->next->size;
            node->next = node->next->next;
        }

        if (previous &&
            get<std::byte>(previous) + sizeof(FLHeader) + previous->size == get<std::byte>(node)) {
            previous->size += sizeof(FLHeader) + node->size;
            previous->next = node->next;
        }
    }

    Node *m_head = nullptr;
};

static constexpr std::size_t KB = 1uz << 10;
static constexpr std::size_t MB = 1uz << 20;
static constexpr std::size_t GB = 1uz << 30;

void bench_linear_v1(benchmark::State &state) {
    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        for (auto i = 0uz; i < KB; ++i) vector[i] = operator new(MB);
        for (auto i = 0uz; i < KB; ++i) operator delete(vector[i], MB);

        benchmark::DoNotOptimize(vector);
    }
}

void bench_linear_v2(benchmark::State &state) {
    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        LinearAllocator allocator(GB);

        for (auto i = 0uz; i < KB; ++i) vector[i] = allocator.allocate(MB);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_linear_v1);
BENCHMARK(bench_linear_v2);

void bench_stack_v1(benchmark::State &state) {
    auto KB = 1uz << 10, MB = 1uz << 20;

    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        for (auto i = 0uz; i < KB; ++i) vector[i] = operator new(MB);
        for (auto i = 0uz; i < KB; ++i) operator delete(vector[std::size(vector) - 1 - i], MB);

        benchmark::DoNotOptimize(vector);
    }
}

void bench_stack_v2(benchmark::State &state) {
    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        StackAllocator allocator(2 * GB);

        for (auto i = 0uz; i < KB; ++i) vector[i] = allocator.allocate(MB);
        for (auto i = 0uz; i < KB; ++i) allocator.deallocate(vector[std::size(vector) - 1 - i]);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_stack_v1);
BENCHMARK(bench_stack_v2);

void bench_list_v1(benchmark::State &state) {
    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        for (auto i = 0uz; i < KB; ++i) vector[i] = operator new(MB);
        for (auto i = 0uz; i < KB; i += 2) operator delete(vector[i], MB);
        for (auto i = 0uz; i < KB; i += 2) vector[i] = operator new(MB);
        for (auto i = 0uz; i < KB; ++i) operator delete(vector[i], MB);

        benchmark::DoNotOptimize(vector);
    }
}

void bench_list_v2(benchmark::State &state) {
    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        ListAllocator allocator(GB, MB);

        for (auto i = 0uz; i < KB; ++i) vector[i] = allocator.allocate();
        for (auto i = 0uz; i < KB; i += 2) allocator.deallocate(vector[i]);
        for (auto i = 0uz; i < KB; i += 2) vector[i] = allocator.allocate();
        for (auto i = 0uz; i < KB; ++i) allocator.deallocate(vector[i]);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_list_v1);
BENCHMARK(bench_list_v2);

void bench_freelist_v1(benchmark::State &state) {
    std::uniform_int_distribution distribution(1, 16);
    std::default_random_engine engine;

    std::vector<std::pair<void *, std::size_t> > vector(KB);

    for (auto _ : state) {
        for (auto i = 0uz; i < KB; ++i) {
            vector[i].second = distribution(engine) * MB;
            vector[i].first = operator new(vector[i].second);
        }
        for (auto i = 0uz; i < KB; i += 32) operator delete(vector[i].first, vector[i].second);
        for (auto i = 0uz; i < KB; i += 32) {
            vector[i].second = distribution(engine) * MB;
            vector[i].first = operator new(vector[i].second);
        }
        for (auto i = 0uz; i < KB; ++i) operator delete(vector[i].first, vector[i].second);

        benchmark::DoNotOptimize(vector);
    }
}

void bench_freelist_v2(benchmark::State &state) {
    std::uniform_int_distribution distribution(1, 16);
    std::default_random_engine engine;

    std::vector<void *> vector(KB, nullptr);

    for (auto _ : state) {
        FreelistAllocator allocator(16 * GB);

        for (auto i = 0uz; i < KB; ++i) vector[i] = allocator.allocate(distribution(engine) * MB);
        for (auto i = 0uz; i < KB; i += 32) allocator.deallocate(vector[i]);
        for (auto i = 0uz; i < KB; i += 32)
            vector[i] = allocator.allocate(distribution(engine) * MB);
        for (auto i = 0uz; i < KB; ++i) allocator.deallocate(vector[i]);

        benchmark::DoNotOptimize(vector);
    }
}

BENCHMARK(bench_freelist_v1);
BENCHMARK(bench_freelist_v2);

int main() {
    std::print("\nLinearAllocator\n");
    {
        LinearAllocator allocator(1 << 10);

        allocator.show();
        allocator.allocate(1, 1);
        allocator.show();
        allocator.allocate(2, 2);
        allocator.show();
        allocator.allocate(4, 4);
        allocator.show();
        allocator.allocate(8, 8);
        allocator.show();
    }

    std::print("\nStackAllocator\n");
    {
        StackAllocator allocator(1 << 10);

        allocator.show();
        allocator.allocate(1, 1);
        allocator.show();
        allocator.allocate(2, 2);
        allocator.show();
        auto x = allocator.allocate(4, 4);
        allocator.show();
        auto y = allocator.allocate(8, 8);

        allocator.show();
        allocator.deallocate(y);
        allocator.show();
        allocator.deallocate(x);

        allocator.show();
        auto z = allocator.allocate(4, 4);
        allocator.show();

        assert(z == x);
    }

    std::print("\nListAllocator\n");
    {
        ListAllocator allocator(64, 32);

        allocator.show();
        allocator.allocate();
        allocator.show();
        auto x = allocator.allocate();
        allocator.show();
        auto y = allocator.allocate();
        allocator.show();
        allocator.allocate();
        allocator.show();
        allocator.allocate();

        allocator.show();
        allocator.deallocate(x);
        allocator.show();
        allocator.deallocate(y);

        allocator.show();
        auto z = allocator.allocate();
        allocator.show();

        assert(z == y);
    }

    std::print("\nFreelistAllocator\n");
    {
        FreelistAllocator allocator(1 << 10);

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

    std::print("\nPolymorphic usage\n");
    {
        LinearAllocator linear(1 << 10);
        StackAllocator stack(1 << 10);
        ListAllocator list(1 << 10, 64);
        FreelistAllocator freelist(1 << 10);

        Allocator *allocators[] = {&linear, &stack, &list, &freelist};

        for (Allocator *a : allocators) {
            void *p1 = a->allocate(64);
            void *p2 = a->allocate(64);
            a->show();
            a->deallocate(p2);
            a->deallocate(p1);
        }
    }
    benchmark::RunSpecifiedBenchmarks();
}
