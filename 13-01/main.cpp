#include <cassert>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

auto to_hex(const std::vector<std::uint8_t>& bytes) {
    std::stringstream ss;

    ss << std::hex << std::setfill('0');

    for (auto byte : bytes) {
        ss << std::setw(2) << std::right << static_cast<int>(byte);
    }

    return ss.str();
}

std::uint8_t hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';

    return c - 'a' + 10;
}

auto from_hex(const std::string& hex) {
    std::vector<std::uint8_t> result;

    for (std::size_t i = 0; i < hex.size(); i += 2) {
        result.push_back(
            static_cast<std::uint8_t>((hex_digit(hex[i]) << 4) | hex_digit(hex[i + 1])));
    }

    return result;
}

int main() {
    std::vector<std::uint8_t> bytes = {0, 1, 127, 128, 255, 0x67, 0xcd};
    auto hex = to_hex(bytes);
    assert(hex == "00017f80ff67cd");
    assert(from_hex(hex) == bytes);
}
