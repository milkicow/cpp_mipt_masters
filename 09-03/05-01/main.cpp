////////////////////////////////////////////////////

// chapter : Patterns

////////////////////////////////////////////////////

// section : Generative Patterns

////////////////////////////////////////////////////

// content : Generative Patterns
//
// content : Pattern Builder

////////////////////////////////////////////////////

#include <cassert>
#include <memory>

////////////////////////////////////////////////////

struct Entity {
    int x = 0, y = 0;
};

////////////////////////////////////////////////////

class Builder {
   public:
    virtual ~Builder() = default;

    auto make_entity() {
        auto entity = std::make_unique<Entity>();

        m_entity = entity.get();

        set_x();
        set_y();

        return entity;
    }

    virtual void set_x() const = 0;
    virtual void set_y() const = 0;

   protected:
    Entity* m_entity = nullptr;
};

////////////////////////////////////////////////////

class Builder_Client : public Builder {
   public:
    void set_x() const override { m_entity->x = 1; }

    void set_y() const override { m_entity->y = 1; }
};

////////////////////////////////////////////////////

class Builder_Server : public Builder {
   public:
    void set_x() const override { m_entity->x = 1; }

    void set_y() const override { m_entity->y = 1; }
};

////////////////////////////////////////////////////

int main() {
    auto builder = std::make_unique<Builder_Client>();
    auto entity = builder->make_entity();
}

////////////////////////////////////////////////////