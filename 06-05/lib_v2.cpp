#include <iostream>

extern "C" {
void test() {
    std::cout << "[Library v2] Hello from version 2!\n";
    std::cout << "[Library v2] Algorithm: computing product of 1..10\n";

    int result = 1;
    for (int i = 1; i <= 10; ++i) result *= i;

    std::cout << "[Library v2] Result = " << result << "\n";
}
}
