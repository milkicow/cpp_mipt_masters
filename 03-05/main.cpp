#include <iostream>
#include <memory>

class Entity_v1 {
public:
    virtual ~Entity_v1() = default;
    virtual void test() = 0;
};

class Entity_v2 {
public:
    virtual ~Entity_v2() = default;
    virtual void test() = 0;
};

class Adapter_v1 : public Entity_v1 {
public:
    ~Adapter_v1() override = default;

    void test() override final {
        test_v1();
    }

    virtual void test_v1() {
        std::cout << "Adapter_v1::test_v1() default implementation\n";
    }
};

class Adapter_v2 : public Entity_v2 {
public:
    ~Adapter_v2() override = default;

    void test() override final {
        test_v2();
    }

    virtual void test_v2() {
        std::cout << "Adapter_v2::test_v2() default implementation\n";
    }
};

class Client : public Adapter_v1, public Adapter_v2 {
public:
    ~Client() override = default;

    void test_v1() override {
        std::cout << "Client::test_v1()\n";
    }

    void test_v2() override {
        std::cout << "Client::test_v2()\n";
    }
};

int main() {
    Client client;

    Entity_v1* p1 = &client;
    std::cout << "Entity_v1*: ";
    p1->test();

    Entity_v2* p2 = &client;
    std::cout << "Entity_v2*: ";
    p2->test();

    std::unique_ptr<Entity_v1> up1 = std::make_unique<Client>();
    std::unique_ptr<Entity_v2> up2 = std::make_unique<Client>();

    std::cout << "unique_ptr<Entity_v1>: ";
    up1->test();

    std::cout << "unique_ptr<Entity_v2>: ";
    up2->test();

    return 0;
}
