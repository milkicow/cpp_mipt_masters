#include "pimpl.hpp"

#include <bit>
#include <memory>
#include <new>
#include <utility>

#include "impl.hpp"

Implementation* Entity::get() noexcept {
    return std::launder(std::bit_cast<Implementation*>(m_buffer.data()));
}

const Implementation* Entity::get() const noexcept {
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
    if (this != &other) {
        std::destroy_at(get());
        ::new (m_buffer.data()) Implementation(std::move(*other.get()));
    }
    return *this;
}
