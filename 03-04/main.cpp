#include <iostream>

class Entity {
   private:
    void test_v1() const { std::cout << "Executing private test_v1()\n"; }

    void test_v2() const { std::cout << "Executing private test_v2()\n"; }

    friend class Attorney1;
    friend class Attorney2;

   public:
    Entity() = default;
};

class Attorney1 {
   private:
    static void call_test_v1(const Entity& e) { e.test_v1(); }
    friend class Tester_v1;
};

class Attorney2 {
   private:
    static void call_test_v2(const Entity& e) { e.test_v2(); }
    friend class Tester_v2;
};

class Tester_v1 {
   public:
    static void run_test(const Entity& e) {
        std::cout << "Tester_v1: calling test_v1()\n";
        Attorney1::call_test_v1(e);
    }
};

class Tester_v2 {
   public:
    static void run_test(const Entity& e) {
        std::cout << "Tester_v2: calling test_v2()\n";
        Attorney2::call_test_v2(e);
    }
};

int main() {
    Entity e;

    Tester_v1::run_test(e);
    Tester_v2::run_test(e);

    return 0;
}
