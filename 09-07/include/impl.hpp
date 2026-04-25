#pragma once
#include <iostream>
#include <utility>

class Implementation {
   public:
    explicit Implementation(int value) : m_value(value) {
        std::cout << "[Implementation] constructed, value = " << m_value << '\n';
    }
    Implementation(Implementation &&other) noexcept : m_value(std::exchange(other.m_value, 0)) {
        std::cout << "[Implementation] move-constructed\n";
    }
    Implementation &operator=(Implementation &&other) noexcept {
        if (this != &other) m_value = std::exchange(other.m_value, 0);
        return *this;
    }
    ~Implementation() { std::cout << "[Implementation] destroyed, value = " << m_value << '\n'; }
    int test() const {
        std::cout << "[Implementation] test(), value = " << m_value << '\n';
        return m_value;
    }

   private:
    int m_value = 0;
};