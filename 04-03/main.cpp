#include <iostream>
#include <vector>

template <typename Container>
void handle(Container& container, int value) {
    container.push_back(value);
}

template <typename Container, typename T>
void handle(Container& container, T&& value) {}

template <typename Container, typename... Args>
void insert_args(Container& container, Args&&... args) {
    (handle(container, std::forward<Args>(args)), ...);
}

int main() {
    std::vector<int> vec;
    insert_args(vec, 1, 3.14, 'a', 5, "hello", 10, 20.0f);

    std::cout << "Vector contents: ";
    for (int val : vec) {
        std::cout << val << " ";
    }
    std::cout << '\n';

    return 0;
}
