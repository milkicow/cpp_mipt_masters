#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <cmath>
#include <iostream>
#include <iterator>
#include <print>
#include <string>
#include <string_view>
#include <vector>

namespace x3 = boost::spirit::x3;

class Operand
    : public x3::variant<double, x3::forward_ast<struct Sign>, x3::forward_ast<struct List> > {
   public:
    using base_type::base_type, base_type::operator=;
};

struct Sign {
    char operation = '\0';
    Operand operand;
};

struct Step {
    char operation = '\0';
    Operand operand;
};

struct List {
    Operand head;

    std::vector<Step> steps;
};

BOOST_FUSION_ADAPT_STRUCT(Sign, operation, operand)

BOOST_FUSION_ADAPT_STRUCT(Step, operation, operand)

BOOST_FUSION_ADAPT_STRUCT(List, head, steps)

namespace parser {
x3::rule<struct rule_1_tag, List> rule_1;

x3::rule<struct rule_2_tag, List> rule_2;

x3::rule<struct rule_power_tag, List> rule_power;

x3::rule<struct rule_post_tag, List> rule_post;

x3::rule<struct rule_3_tag, Operand> rule_3;

auto rule_1_def = rule_2 >> *(x3::char_('+') >> rule_2 | x3::char_('-') >> rule_2);

auto rule_2_def = rule_power >> *(x3::char_('*') >> rule_power | x3::char_('/') >> rule_power |
                                  x3::char_('%') >> rule_power);

auto rule_power_def = rule_post >> *(x3::char_('^') >> rule_post);

auto rule_post_def = rule_3 >> *(x3::char_('!') >> x3::attr(0.0));

auto rule_3_def = (x3::char_('+') >> rule_3 | x3::char_('-') >> rule_3 | x3::double_ |
                   '(' >> rule_1 >> ')' | '[' >> rule_1 >> ']' | '{' >> rule_1 >> '}');

BOOST_SPIRIT_DEFINE(rule_1, rule_2, rule_power, rule_post, rule_3)
}  // namespace parser

class Calculator {
   public:
    auto operator()(double x) const -> double { return x; }

    auto operator()(Sign const& sign) const -> double {
        auto x = boost::apply_visitor(*this, sign.operand);

        switch (sign.operation) {
            case '+': {
                return x;
            }
            case '-': {
                return -1 * x;
            }
        }

        return x;
    }

    auto operator()(Step const& step, double x) const -> double {
        auto y = boost::apply_visitor(*this, step.operand);

        switch (step.operation) {
            case '+': {
                return x + y;
            }
            case '-': {
                return x - y;
            }
            case '*': {
                return x * y;
            }
            case '/': {
                return x / y;
            }
            case '%': {
                return std::fmod(x, y);
            }
            case '^': {
                return std::pow(x, y);
            }
            case '!': {
                auto result = 1.0;

                for (auto i = 2; i <= static_cast<int>(x); ++i) result *= i;

                return result;
            }
        }

        return x;
    }

    auto operator()(List const& list) const -> double {
        auto x = boost::apply_visitor(*this, list.head);

        for (auto const& step : list.steps) {
            x = (*this)(step, x);
        }

        return x;
    }
};

auto parse(std::string_view view) {
    List list;

    x3::phrase_parse(std::begin(view), std::end(view), parser::rule_1, x3::ascii::space, list);

    static Calculator calculator;

    return calculator(list);
}

int main() {
    std::string string;

    std::print("main : enter std::string string(s) : \n");

    while (std::getline(std::cin >> std::ws, string, '\n')) {
        if (string.front() != ';') {
            std::print("main : {} = {}\n", string, parse(string));
        } else {
            break;
        }
    }
}
