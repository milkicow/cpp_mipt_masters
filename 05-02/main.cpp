#include <iostream>

class Entity {
   public:
    virtual ~Entity() = default;
    virtual void operation() const = 0;
};

template <typename Base>
class Decorator : virtual public Entity, public Base {
   public:
    ~Decorator() override = default;
    void operation() const override { Base::operation(); }
};

class Base1 : public virtual Entity {
   public:
    ~Base1() override = default;
    void operation() const override { std::cout << "Base1 operation\n"; }
};

class Base2 : public virtual Entity {
   public:
    ~Base2() override = default;
    void operation() const override { std::cout << "Base2 operation\n"; }
};

int main() {
    Decorator<Base1> decoratedBase1;
    decoratedBase1.operation();

    Decorator<Base2> decoratedBase2;
    decoratedBase2.operation();
    return 0;
}