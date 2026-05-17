#include <cassert>
#include <cctype>
#include <cmath>
#include <fstream>
#include <limits>
#include <print>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

class Stream {
   public:
    using token_t = std::variant<char, double, std::string>;

    Stream(std::string const& string) : m_stream(string + ';') {}

    auto empty() { return m_stream.peek() == ';'; }

    auto get() -> token_t {
        if (m_has_token) {
            m_has_token = false;

            return m_token;
        }

        auto x = '\0';

        m_stream >> x;

        switch (x) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '!':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ';': {
                return token_t(x);
            }

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':

            case '5':
            case '6':
            case '7':
            case '8':
            case '9':

            case '.': {
                m_stream.unget();

                auto y = 0.0;

                m_stream >> y;

                return token_t(y);
            }

            default: {
                std::string string(1, x);

                while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x))) {
                    string += x;
                }

                if (!std::isspace(x)) {
                    m_stream.unget();
                }

                return token_t(string);
            }
        }
    }

    void put(token_t const& token) {
        m_token = token;

        m_has_token = true;
    }

   private:
    std::stringstream m_stream;

    token_t m_token;

    bool m_has_token = false;
};

class Calculator {
   public:
    auto evaluate(std::string const& string) -> double {
        Stream stream(string);

        return statement(stream);
    }

   private:
    auto statement(Stream& stream) -> double {
        auto token = stream.get();

        if (std::holds_alternative<std::string>(token)) {
            if (std::get<std::string>(token) == "set") {
                return declaration(stream);
            }
        }

        stream.put(token);

        return expression(stream);
    }

    auto declaration(Stream& stream) -> double {
        auto string = std::get<std::string>(stream.get());

        m_variables[string] = expression(stream);

        return m_variables[string];
    }

    auto expression(Stream& stream) const -> double {
        auto x = term(stream);

        auto token = stream.get();

        while (true) {
            if (!std::holds_alternative<char>(token)) {
                stream.put(token);

                return x;
            }

            switch (std::get<char>(token)) {
                case '+': {
                    x += term(stream);
                    break;
                }

                case '-': {
                    x -= term(stream);
                    break;
                }

                default: {
                    stream.put(token);

                    return x;
                }
            }

            token = stream.get();
        }
    }

    auto term(Stream& stream) const -> double {
        auto x = power(stream);

        auto token = stream.get();

        while (true) {
            if (!std::holds_alternative<char>(token)) {
                stream.put(token);

                return x;
            }

            switch (std::get<char>(token)) {
                case '*': {
                    x *= power(stream);
                    break;
                }

                case '/': {
                    x /= power(stream);
                    break;
                }

                case '%': {
                    x = std::fmod(x, power(stream));
                    break;
                }

                default: {
                    stream.put(token);

                    return x;
                }
            }

            token = stream.get();
        }
    }

    auto power(Stream& stream) const -> double {
        auto x = postfix(stream);

        auto token = stream.get();

        if (std::holds_alternative<char>(token) && std::get<char>(token) == '^') {
            return std::pow(x, power(stream));
        }

        stream.put(token);

        return x;
    }

    auto postfix(Stream& stream) const -> double {
        auto x = primary(stream);

        auto token = stream.get();

        while (std::holds_alternative<char>(token) && std::get<char>(token) == '!') {
            auto result = 1.0;

            for (auto i = 2; i <= static_cast<int>(x); ++i) result *= i;

            x = result;

            token = stream.get();
        }

        stream.put(token);

        return x;
    }

    auto primary(Stream& stream) const -> double {
        auto token = stream.get();

        if (std::holds_alternative<char>(token)) {
            switch (std::get<char>(token)) {
                case '(': {
                    auto x = expression(stream);
                    stream.get();
                    return x;
                }

                case '[': {
                    auto x = expression(stream);
                    stream.get();
                    return x;
                }

                case '{': {
                    auto x = expression(stream);
                    stream.get();
                    return x;
                }

                case '+': {
                    return primary(stream);
                }

                case '-': {
                    return -1 * primary(stream);
                }
            }
        }

        if (std::holds_alternative<double>(token)) {
            return std::get<double>(token);
        }

        return m_variables.at(std::get<std::string>(token));
    }

    std::unordered_map<std::string, double> m_variables;
};

int main() {
    std::fstream file("test.txt", std::ios::in);

    Calculator calc;

    std::string expression;

    while (std::getline(file, expression)) {
        auto expected = 0.0;

        file >> expected;

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        auto result = calc.evaluate(expression);

        assert(std::abs(result - expected) < 1e-9);

        std::print("{} = {}\n", expression, result);
    }
}
