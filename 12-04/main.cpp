#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

using namespace std::literals;

int main() {
    auto text = R"(
        Contact us at support@example.com or admin@test.org.
        You can also reach john.doe+tag@company.co.uk for help.
        Invalid: not-an-email, @nodomain, noatsign.com
    )"s;

    std::regex pattern(R"([a-zA-Z0-9._+-]+@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");

    std::vector<std::string> emails;
    std::vector<std::string> domains;

    std::ranges::for_each(std::sregex_iterator(std::cbegin(text), std::cend(text), pattern),
                          std::sregex_iterator(), [&emails, &domains](const auto& match) {
                              emails.push_back(match[0]);
                              domains.push_back(match[1]);
                          });

    assert((emails == std::vector<std::string>{"support@example.com", "admin@test.org",
                                               "john.doe+tag@company.co.uk"}));

    assert((domains == std::vector<std::string>{"example.com", "test.org", "company.co.uk"}));
}
