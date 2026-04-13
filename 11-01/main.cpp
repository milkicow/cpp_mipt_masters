#include <iostream>

using FuncPtr = void (*)();

class Wrapper {
   private:
    FuncPtr func;

   public:
    Wrapper(FuncPtr f) : func(f) {}
    operator FuncPtr() const { return func; }
};

void real_function() { std::cout << "Hello from real_function!" << std::endl; }
Wrapper test() { return Wrapper(real_function); }

int main() {
    Wrapper function = test();
    (*function)();

    return 0;
}
