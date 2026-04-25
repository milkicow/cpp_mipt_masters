#pragma once
#include <array>
#include <boost/noncopyable.hpp>
#include <cstddef>

class Implementation;

class Entity : private boost::noncopyable {
   public:
    Entity();
    Entity(Entity&& other);
    Entity& operator=(Entity&& other);
    ~Entity();

   public:
    Implementation* get() noexcept;
    const Implementation* get() const noexcept;

   private:
    alignas(std::max_align_t) std::array<std::byte, 16> m_buffer;
};
