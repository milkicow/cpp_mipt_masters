#include <boost/dll.hpp>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
int main() {
    std::string path;

    std::cout << "Enter path to shared library: ";
    std::cin >> path;

    try {
        std::function<void()> fn = boost::dll::import_symbol<void()>(path, "test");
        fn();
    } catch (std::exception const& error) {
        std::cerr << "Error: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
