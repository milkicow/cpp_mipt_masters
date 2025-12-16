#include <iostream>

class Strategy1 {
   public:
    void echo(const std::string &str) const {
        std::cout << "Echo Strategy 1: " << str << std::endl;
    };
};

class Strategy2 {
   public:
    void echo(const std::string &str) const {
        std::cout << "Echo Strategy 2: " << str << std::endl;
    };
};

template <typename Strategy>
class Entity : public Strategy {
   private:
    std::string field;

   public:
    Entity(const std::string &f) : field(f) {}
    void echo() const { Strategy::echo(field); }
};

int main() {
    Entity<Strategy1> entity1("Hello");
    Entity<Strategy2> entity2("World");

    entity1.echo();
    entity2.echo();

    return 0;
}