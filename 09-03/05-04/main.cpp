///////////////////////////////////////////////////////////

// chapter : Patterns

///////////////////////////////////////////////////////////

// section : Generative Patterns

///////////////////////////////////////////////////////////

// content : Pattern Prototype
//
// content : Virtual Constructors

///////////////////////////////////////////////////////////

#include <memory>
#include <vector>

///////////////////////////////////////////////////////////

class Entity {
   public:
    virtual ~Entity() = default;

    //  ----------------------------------

    virtual std::unique_ptr<Entity> copy() const = 0;
};

///////////////////////////////////////////////////////////

class Client : public Entity {
   public:
    std::unique_ptr<Entity> copy() const override { return std::make_unique<Client>(); }
};

///////////////////////////////////////////////////////////

class Server : public Entity {
   public:
    std::unique_ptr<Entity> copy() const override { return std::make_unique<Server>(); }
};

///////////////////////////////////////////////////////////

class Prototype {
   public:
    Prototype() {
        m_entities.push_back(std::make_unique<Client>());
        m_entities.push_back(std::make_unique<Server>());
    }
    auto make_client() { return m_entities.at(0)->copy(); }
    auto make_server() { return m_entities.at(1)->copy(); }

   private:
    std::vector<std::unique_ptr<Entity>> m_entities;
};

///////////////////////////////////////////////////////////

int main() {
    Prototype().make_client();
}

///////////////////////////////////////////////////////////