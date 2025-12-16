#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>

class IPv4 {
   private:
    std::array<std::uint8_t, 4> m_addr{};

    std::uint32_t to_uint32() const {
        return (static_cast<std::uint32_t>(m_addr[0]) << 24) |
               (static_cast<std::uint32_t>(m_addr[1]) << 16) |
               (static_cast<std::uint32_t>(m_addr[2]) << 8) |
               (static_cast<std::uint32_t>(m_addr[3]));
    }

    void from_uint32(std::uint32_t value) {
        m_addr[0] = static_cast<std::uint8_t>((value >> 24) & 0xFF);
        m_addr[1] = static_cast<std::uint8_t>((value >> 16) & 0xFF);
        m_addr[2] = static_cast<std::uint8_t>((value >> 8) & 0xFF);
        m_addr[3] = static_cast<std::uint8_t>(value & 0xFF);
    }

   public:
    IPv4() = default;

    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d) : m_addr{a, b, c, d} {}

    IPv4& operator++() {
        std::uint32_t value = to_uint32();
        ++value;
        from_uint32(value);
        return *this;
    }

    IPv4 operator++(int) {
        IPv4 temp = *this;
        ++(*this);
        return temp;
    }

    IPv4& operator--() {
        std::uint32_t value = to_uint32();
        --value;
        from_uint32(value);
        return *this;
    }

    IPv4 operator--(int) {
        IPv4 temp = *this;
        --(*this);
        return temp;
    }

    bool operator<=>(const IPv4& other) const = default;

    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
        os << static_cast<int>(ip.m_addr[0]) << '.' << static_cast<int>(ip.m_addr[1]) << '.'
           << static_cast<int>(ip.m_addr[2]) << '.' << static_cast<int>(ip.m_addr[3]);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, IPv4& ip) {
        int a, b, c, d;
        char dot1, dot2, dot3;

        if (is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) {
            if (dot1 == '.' && dot2 == '.' && dot3 == '.' && a >= 0 && a <= 255 && b >= 0 &&
                b <= 255 && c >= 0 && c <= 255 && d >= 0 && d <= 255) {
                ip.m_addr = {static_cast<std::uint8_t>(a), static_cast<std::uint8_t>(b),
                             static_cast<std::uint8_t>(c), static_cast<std::uint8_t>(d)};
            } else {
                is.setstate(std::ios::failbit);
            }
        }
        return is;
    }
};

int main() {
    IPv4 ip1;
    std::cout << "input IP-адрес (e.g. 192.168.1.1): ";
    std::cin >> ip1;

    if (!std::cin) {
        std::cerr << "incorrect input" << std::endl;
        return 1;
    }

    std::cout << "Enter address: " << ip1 << std::endl;

    IPv4 ip2 = ip1++;
    std::cout << "After postfix inc: " << ip1 << " (old val: " << ip2 << ")"
              << std::endl;
    ++ip1;

    std::cout << "After prefix inc: " << ip1 << std::endl;

    ip1--;
    std::cout << "After postfix dec: " << ip1 << std::endl;

    --ip1;
    std::cout << "After prefix dec: " << ip1 << std::endl;

    return 0;
}
