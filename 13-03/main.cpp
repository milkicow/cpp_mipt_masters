#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <sstream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////

void transform(std::string const& path_1, std::string const& path_2) {
    auto s = (std::stringstream() << std::fstream(path_1, std::ios::in).rdbuf()).str();

    for (auto it = std::begin(s); it != std::end(s); ++it) {
        if (*it == '\'') {
            do {
                ++it;
            } while (!(*it == '\'' && *std::prev(it) != '\\'));
        } else if (*it == 'R' && std::next(it) != std::end(s) && *std::next(it) == '"') {
            ++it;
            ++it;

            std::string end_seq = ")";

            while (it != std::end(s) && *it != '(') {
                end_seq += *it;
                ++it;
            }

            end_seq += '"';

            if (it != std::end(s)) ++it;

            while (it != std::end(s)) {
                auto pos = static_cast<std::size_t>(it - std::begin(s));

                if (s.compare(pos, end_seq.size(), end_seq) == 0) {
                    std::advance(it, end_seq.size() - 1);
                    break;
                }

                ++it;
            }
        } else if (*it == '"') {
            do {
                ++it;
            } while (!(*it == '"' && *std::prev(it) != '\\'));
        } else if (*it == '/') {
            if (std::next(it) != std::end(s) && *std::next(it) == '/') {
                auto end = std::next(it, 2);

                while (end != std::end(s) && *end != '\n') {
                    ++end;
                }

                it = s.erase(it, end);
            } else if (std::next(it) != std::end(s) && *std::next(it) == '*') {
                auto end = std::next(it, 3);

                while (end != std::end(s) && !(*end == '/' && *std::prev(end) == '*')) {
                    ++end;
                }

                if (end != std::end(s)) it = s.erase(it, ++end);
            }
        }

        if (it == std::end(s)) break;
    }

    std::string result;
    std::string line;
    bool in_raw = false;
    std::string raw_end;

    auto it = std::cbegin(s);

    while (it != std::cend(s)) {
        if (!in_raw && *it == '\'') {
            line += *it;
            do {
                ++it;
                line += *it;
            } while (!(*it == '\'' && *std::prev(it) != '\\'));
            ++it;
            continue;
        }

        if (!in_raw && *it == 'R' && std::next(it) != std::cend(s) && *std::next(it) == '"') {
            in_raw = true;
            raw_end = ")";
            line += *it++;  // R
            line += *it++;  // "
            while (it != std::cend(s) && *it != '(') {
                raw_end += *it;
                line += *it++;
            }
            raw_end += '"';
            if (it != std::cend(s)) line += *it++;
            continue;
        }

        if (in_raw) {
            line += *it;

            if (line.size() >= raw_end.size() &&
                line.compare(line.size() - raw_end.size(), raw_end.size(), raw_end) == 0) {
                in_raw = false;
            }

            if (*it == '\n') {
                result += line;
                line = "";
            }

            ++it;
            continue;
        }

        if (!in_raw && *it == '"') {
            line += *it;
            do {
                ++it;
                line += *it;
            } while (!(*it == '"' && *std::prev(it) != '\\'));
            ++it;
            continue;
        }

        if (*it == '\n') {
            if (!std::ranges::all_of(line, [](char c) { return std::isspace(c); })) {
                result += line.substr(0, line.find_last_not_of(" \t\r") + 1) + '\n';
            }

            line = "";
        } else {
            line += *it;
        }

        ++it;
    }

    if (!line.empty() && !std::ranges::all_of(line, [](char c) { return std::isspace(c); })) {
        result += line.substr(0, line.find_last_not_of(" \t\r") + 1);
    }

    std::fstream(path_2, std::ios::out) << result;
}

int main() {
    auto path_1 = "source.cpp";
    auto path_2 = "output.cpp";

    transform(path_1, path_2);

    std::print("main : enter char : ");
    std::cin.get();

    std::filesystem::remove(path_2);
}
