#include <cassert>
#include <cmath>
#include <compare>
#include <iostream>
#include <istream>
#include <numeric>
#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <vector>

template <typename Derived>
struct addable {
    friend Derived operator+(Derived lhs, Derived const& rhs) { return lhs += rhs; }
};
template <typename Derived>
struct subtractable {
    friend Derived operator-(Derived lhs, Derived const& rhs) { return lhs -= rhs; }
};
template <typename Derived>
struct multipliable {
    friend Derived operator*(Derived lhs, Derived const& rhs) { return lhs *= rhs; }
};
template <typename Derived>
struct dividable {
    friend Derived operator/(Derived lhs, Derived const& rhs) { return lhs /= rhs; }
};
template <typename Derived>
struct incrementable {
    friend Derived& operator++(Derived& x) { return x += Derived(1); }
    friend Derived operator++(Derived& x, int) {
        Derived old = x;
        ++x;
        return old;
    }
};
template <typename Derived>
struct decrementable {
    friend Derived& operator--(Derived& x) { return x -= Derived(1); }
    friend Derived operator--(Derived& x, int) {
        Derived old = x;
        --x;
        return old;
    }
};

class Exception : public std::exception {
   public:
    explicit Exception(std::string message) : m_message(std::move(message)) {}

    const char* what() const noexcept override { return m_message.c_str(); }

   private:
    std::string m_message;
};

template <std::integral T>
class Rational : public addable<Rational<T>>,
                 public subtractable<Rational<T>>,
                 public multipliable<Rational<T>>,
                 public dividable<Rational<T>>,
                 public incrementable<Rational<T>>,
                 public decrementable<Rational<T>> {
   public:
    /* explicit */ Rational(T num = 0, T den = 1) : m_num(num), m_den(den) {
        if (m_den == T(0)) throw Exception("Rational: denominator must not be zero");
        reduce();
    }

    explicit operator double() const { return 1.0 * m_num / m_den; }

    auto& operator+=(Rational const& other) {
        auto lcm = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;
        reduce();
        return *this;
    }
    auto& operator-=(Rational const& other) {
        return *this += Rational(other.m_num * T(-1), other.m_den);
    }
    auto& operator*=(Rational const& other) {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }
    auto& operator/=(Rational const& other) {
        // Деление на ноль: конструктор Rational(other.m_den, other.m_num)
        // сам бросит Exception, если other.m_num == 0
        return *this *= Rational(other.m_den, other.m_num);
    }

    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs) {
        long long left = static_cast<long long>(lhs.m_num) * rhs.m_den;
        long long right = static_cast<long long>(rhs.m_num) * lhs.m_den;
        if (left < right) return std::strong_ordering::less;
        if (left > right) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
    friend bool operator==(Rational const& lhs, Rational const& rhs) {
        return lhs.m_num == rhs.m_num && lhs.m_den == rhs.m_den;
    }

    friend auto& operator>>(std::istream& stream, Rational& r) {
        return (stream >> r.m_num).ignore() >> r.m_den;
    }
    friend auto& operator<<(std::ostream& stream, Rational const& r) {
        return stream << r.m_num << '/' << r.m_den;
    }

   private:
    void reduce() {
        if (m_den < 0) {
            m_num = -m_num;
            m_den = -m_den;
        }
        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }
    T m_num = 0, m_den = 1;
};

auto equal(double x, double y, double epsilon = 1e-6) { return std::abs(x - y) < epsilon; }

int main() {
    try {
        Rational<int> x = 1, y(2, 1);

        assert(equal(static_cast<double>(x), 1));

        assert((x += y) == Rational<int>(+3, 1));
        assert((x -= y) == Rational<int>(+1, 1));
        assert((x *= y) == Rational<int>(+2, 1));
        assert((x /= y) == Rational<int>(+1, 1));

        assert((x++) == Rational<int>(+1, 1));
        assert((x--) == Rational<int>(+2, 1));
        assert((++y) == Rational<int>(+3, 1));
        assert((--y) == Rational<int>(+2, 1));

        assert((x + y) == Rational<int>(+3, 1));
        assert((x - y) == Rational<int>(-1, 1));
        assert((x * y) == Rational<int>(+2, 1));
        assert((x / y) == Rational<int>(+1, 2));

        assert((x += 1) == Rational<int>(+2, 1));
        assert((x + 1) == Rational<int>(+3, 1));
        assert((1 + y) == Rational<int>(+3, 1));
        assert((1 + 1) == Rational<int>(+2, 1));

        assert((x < y) == 0);
        assert((x > y) == 0);
        assert((x <= y) == 1);
        assert((x >= y) == 1);
        assert((x == y) == 1);
        assert((x != y) == 0);

        std::stringstream stream_1("1/2");
        std::stringstream stream_2;
        stream_1 >> x;
        stream_2 << x;
        assert(stream_2.str() == stream_1.str());

        Rational<long long> a(1000000000LL, 500000000LL);
        Rational<short> b(3, 4);
        Rational<long> c(5, 6);

        assert((a + a) == Rational<long long>(4, 1));
        assert((b * b) == Rational<short>(9, 16));

        // ── Exception: нулевой знаменатель ───────────────────────────────────
        // Rational(T num, T den) бросает Exception, потому что знаменатель
        // равен нулю.
        Rational<int> bad(1, 0);

    } catch (Exception const& e) {
        std::cerr << "[Exception] " << e.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "[std::exception] " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[unknown exception]\n";
    }

    // ── std::bad_alloc ───────────────────────────────────────────────────────
    // std::vector пытается выделить блок памяти размером, заведомо
    // превышающим объём доступной оперативной памяти (SIZE_MAX байт).
    // Аллокатор не может выполнить запрос и бросает std::bad_alloc.
    try {
        std::vector<int> huge(std::numeric_limits<std::size_t>::max());
    } catch (std::bad_alloc const& e) {
        std::cerr << "[std::bad_alloc] " << e.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "[std::exception] " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[unknown exception]\n";
    }

    // ── std::length_error ────────────────────────────────────────────────────
    // std::vector::resize бросает std::length_error, если запрошенный размер
    // превышает max_size() контейнера (максимальный теоретически допустимый
    // размер, который вектор способен хранить согласно стандарту).
    try {
        std::vector<int> v;
        v.resize(v.max_size() + 1);  // max_size()+1 > max_size() → length_error
    } catch (std::length_error const& e) {
        std::cerr << "[std::length_error] " << e.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "[std::exception] " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[unknown exception]\n";
    }

    // ── std::out_of_range ────────────────────────────────────────────────────
    // std::vector::at выполняет проверку границ и бросает std::out_of_range,
    // если переданный индекс >= size() вектора (в отличие от operator[],
    // который не проверяет границы).
    try {
        std::vector<int> v = {1, 2, 3};
        [[maybe_unused]] auto val = v.at(100);
    } catch (std::out_of_range const& e) {
        std::cerr << "[std::out_of_range] " << e.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "[std::exception] " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[unknown exception]\n";
    }

    // ── std::bad_variant_access ──────────────────────────────────────────────
    // std::get<T>(variant) бросает std::bad_variant_access, если вариант
    // в данный момент хранит значение другого типа (здесь хранится int,
    // а мы пытаемся извлечь double).
    try {
        std::variant<int, double> var = 52;
        [[maybe_unused]] auto val = std::get<double>(var);
    } catch (std::bad_variant_access const& e) {
        std::cerr << "[std::bad_variant_access] " << e.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "[std::exception] " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[unknown exception]\n";
    }

    // ── std::bad_optional_access ─────────────────────────────────────────────
    // std::optional::value() бросает std::bad_optional_access, если опционал
    // пуст (не содержит значения). В отличие от operator*, value() безопасен.
    try {
        std::optional<int> empty;
        [[maybe_unused]] auto val = empty.value();
    } catch (std::bad_optional_access const& e) {
        std::cerr << "[std::bad_optional_access] " << e.what() << '\n';
    } catch (std::exception const& e) {
        std::cerr << "[std::exception] " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[unknown exception]\n";
    }

    return 0;
}
