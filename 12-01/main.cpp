#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>

int main() {
    constexpr long double rate = 77.9L;

    std::stringstream in;
    in.imbue(std::locale("ru_RU.UTF-8"));
    in.str("235000,00 RUB");

    long double kopecks = 0;
    in >> std::get_money(kopecks, true);

    const long double cents = kopecks / rate;

    std::stringstream out;
    out.imbue(std::locale("en_US.UTF-8"));
    out << std::put_money(cents, true);

    std::cout << "RUB: " << kopecks / 100 << '\n';
    std::cout << "USD: " << out.str() << '\n';

    return 0;
}
