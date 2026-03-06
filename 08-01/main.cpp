#include <cstddef>
#include <cstring>
#include <iostream>

#define PRINT_FIELD(label, val) std::cout << (label) << " = " << (val) << '\n'

class Entity_v1 {
    int secret;

   public:
    explicit Entity_v1(int v) : secret(v) {}
    int get() const { return secret; }
};

class Entity_v2 {
   public:
    int secret;
};
void method1_reinterpret_cast() {
    std::cout << "=== Method 1: reinterpret_cast ===\n";

    Entity_v1 e(52);
    PRINT_FIELD("before e.get()", e.get());

    Entity_v2& alias = reinterpret_cast<Entity_v2&>(e);
    alias.secret = 100;

    PRINT_FIELD("after  e.get()", e.get());
    std::cout << '\n';
}

struct Entity_v1_mirror {
    int secret;
};

void method2_pointer_arithmetic() {
    std::cout << "=== Method 2: offsetof ===\n";

    Entity_v1 e(77);
    PRINT_FIELD("before e.get()", e.get());

    constexpr std::size_t offset = offsetof(Entity_v1_mirror, secret);

    char* base = reinterpret_cast<char*>(&e);
    int* field = reinterpret_cast<int*>(base + offset);
    *field = 200;

    PRINT_FIELD("after  e.get()", e.get());
    std::cout << '\n';
}

struct Entity_v1_pod {
    int secret;
};
static_assert(sizeof(Entity_v1) == sizeof(Entity_v1_pod));

void method3_memcpy() {
    std::cout << "=== Method 3: memcpy ===\n";

    Entity_v1 e(333);
    PRINT_FIELD("before e.get()", e.get());

    Entity_v1_pod pod;
    std::memcpy(&pod, &e, sizeof(pod));

    pod.secret = 999;

    std::memcpy(&e, &pod, sizeof(e));

    PRINT_FIELD("after  e.get()", e.get());
    std::cout << '\n';
}

int main() {
    method1_reinterpret_cast();
    method2_pointer_arithmetic();
    method3_memcpy();
    return 0;
}