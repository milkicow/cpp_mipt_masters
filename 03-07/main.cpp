////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <print>
#include <utility>

////////////////////////////////////////////////////////////////////////////////////

class Vector {
   public:
    // ---------------------------------------------------------------------------

    Vector() : m_array(nullptr), m_size(0), m_capacity(0) { std::print("Vector::Vector (1)\n"); }

    // ---------------------------------------------------------------------------

    Vector(std::initializer_list<int> list) : m_size(std::size(list)), m_capacity(std::size(list)) {
        std::print("Vector::Vector (2)\n");

        m_array = m_capacity ? new int[m_capacity]{} : nullptr;

        std::ranges::copy(list, m_array);
    }

    // ---------------------------------------------------------------------------

    Vector(Vector const& other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        std::print("Vector::Vector (3)\n");

        m_array = m_capacity ? new int[m_capacity]{} : nullptr;

        std::ranges::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

    // ---------------------------------------------------------------------------

    Vector(Vector&& other)
        : m_array(std::exchange(other.m_array, nullptr)),
          m_size(std::exchange(other.m_size, 0)),
          m_capacity(std::exchange(other.m_capacity, 0)) {
        std::print("Vector::Vector (4)\n");
    }

    // ---------------------------------------------------------------------------

    ~Vector() {
        std::print("Vector::~Vector\n");
        delete[] m_array;
    }

    // ---------------------------------------------------------------------------

    auto& operator=(Vector other) {
        std::print("Vector::operator= (copy & swap)\n");
        swap(other);
        return *this;
    }

    // ---------------------------------------------------------------------------

    void swap(Vector& other) {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    // ---------------------------------------------------------------------------
    // Новые методы по заданию

    [[nodiscard]] std::size_t size() const noexcept { return m_size; }

    [[nodiscard]] std::size_t capacity() const noexcept { return m_capacity; }

    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }

    // ---------------------------------------------------------------------------
    // Добавление элемента в конец (с возможным увеличением ёмкости)

    void push_back(int value) {
        if (m_size >= m_capacity) {
            // Алгоритм увеличения ёмкости (пример из задачи 02.07)
            std::size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;

            std::print("Vector::push_back -> grow capacity from {} to {}\n", m_capacity,
                       new_capacity);

            int* new_array = new int[new_capacity]{};

            if (m_array) {
                std::ranges::copy(m_array, m_array + m_size, new_array);
                delete[] m_array;
            }

            m_array = new_array;
            m_capacity = new_capacity;
        }

        m_array[m_size++] = value;
    }

    // ---------------------------------------------------------------------------
    // Очистка элементов без освобождения памяти

    void clear() noexcept { m_size = 0; }

   private:
    int* m_array = nullptr;
    std::size_t m_size = 0;
    std::size_t m_capacity = 0;
};

////////////////////////////////////////////////////////////////////////////////////

void swap(Vector& lhs, Vector& rhs) { lhs.swap(rhs); }

////////////////////////////////////////////////////////////////////////////////////

int main() {
    Vector vector_1;
    Vector vector_2 = {1, 2, 3, 4, 5};
    Vector vector_3 = vector_2;
    Vector vector_4 = std::move(vector_3);

    // --------------------------------------

    vector_3 = vector_2;
    vector_4 = std::move(vector_3);

    // --------------------------------------

    swap(vector_1, vector_2);

    // --------------------------------------
    std::print("\n=== push_back demo ===\n");

    Vector v;
    for (int i = 1; i <= 10; ++i) {
        v.push_back(i);
        std::print("After push_back({}): size = {}, capacity = {}\n", i, v.size(), v.capacity());
    }

    // Очистка
    v.clear();
    std::print("\nAfter clear(): size = {}, capacity = {}, empty = {}\n", v.size(), v.capacity(),
               v.empty());
}

////////////////////////////////////////////////////////////////////////////////////
