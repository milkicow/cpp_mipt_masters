#include <iostream>

extern "C" {
void test() {
    std::cout << "[Library v1] Hello from version 1!\n";
    std::cout << "[Library v1] Algorithm: computing sum of 1..10\n";

    int result = 0;
    for (int i = 1; i <= 10; ++i) result += i;

    std::cout << "[Library v1] Result = " << result << "\n";
}
}
