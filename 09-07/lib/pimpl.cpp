#include "pimpl.hpp"

#include <bit>
#include <iostream>
#include <memory>
#include <new>
#include <utility>

class Entity::Implementation {
   public:
    explicit Implementation(int value) : m_value(value) {
        std::cout << "[Implementation] constructed, value = " << m_value << '\n';
    }

    Implementation(Implementation&& other) noexcept : m_value(std::exchange(other.m_value, 0)) {
        std::cout << "[Implementation] move-constructed\n";
    }

    Implementation& operator=(Implementation&& other) noexcept {
        if (this != &other) m_value = std::exchange(other.m_value, 0);
        return *this;
    }

    ~Implementation() { std::cout << "[Implementation] destroyed, value = " << m_value << '\n'; }

    auto test() const {
        std::cout << "[Implementation] test(), value = " << m_value << '\n';
        return m_value;
    }

   private:
    int m_value = 0;
};

Entity::Implementation* Entity::get() noexcept {
    return std::launder(std::bit_cast<Implementation*>(m_buffer.data()));
}

const Entity::Implementation* Entity::get() const noexcept {
    return std::launder(std::bit_cast<const Implementation*>(m_buffer.data()));
}

Entity::Entity() {
    static_assert(sizeof(Implementation) <= sizeof(m_buffer));
    static_assert(alignof(Implementation) <= alignof(std::max_align_t));

    ::new (m_buffer.data()) Implementation(0x67);
}

Entity::Entity(Entity&& other) {
    static_assert(sizeof(Implementation) <= sizeof(m_buffer));
    static_assert(alignof(Implementation) <= alignof(std::max_align_t));

    ::new (m_buffer.data()) Implementation(std::move(*other.get()));
}

Entity::~Entity() { std::destroy_at(get()); }

Entity& Entity::operator=(Entity&& other) {
    if (this != &other) *get() = std::move(*other.get());
    return *this;
}

int Entity::test() const { return get()->test(); }
