#pragma once

#include <compare>
#include <iosfwd>
#include <numeric>

class Rational {
   public:
    /* explicit */ Rational(int num = 0, int den = 1) : m_num(num), m_den(den) { reduce(); }

    explicit operator double() const { return static_cast<double>(m_num) / m_den; }

    Rational& operator+=(Rational const& other);
    Rational& operator-=(Rational const& other);
    Rational& operator*=(Rational const& other);
    Rational& operator/=(Rational const& other);

    Rational const operator++(int);
    Rational const operator--(int);

    Rational& operator++();
    Rational& operator--();

    friend Rational operator+(Rational lhs, Rational const& rhs) { return lhs += rhs; }

    friend Rational operator-(Rational lhs, Rational const& rhs) { return lhs -= rhs; }

    friend Rational operator*(Rational lhs, Rational const& rhs) { return lhs *= rhs; }

    friend Rational operator/(Rational lhs, Rational const& rhs) { return lhs /= rhs; }

    friend std::strong_ordering operator<=>(Rational const& lhs, Rational const& rhs);

    friend bool operator==(Rational const& lhs, Rational const& rhs);

    friend std::istream& operator>>(std::istream& stream, Rational& rational);
    friend std::ostream& operator<<(std::ostream& stream, Rational const& rational);

   private:
    void reduce();

    int m_num = 0;
    int m_den = 1;
};

bool equal(double x, double y, double epsilon = 1e-6);
