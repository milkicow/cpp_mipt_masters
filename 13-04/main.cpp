#include <CLI/CLI.hpp>
#include <chrono>
#include <filesystem>
#include <format>
#include <iterator>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

auto make_type(std::filesystem::file_status const& status) {
    if (std::filesystem::is_directory(status)) return 'd';
    if (std::filesystem::is_regular_file(status)) return 'f';
    if (std::filesystem::is_symlink(status)) return 'l';
    return '?';
}

auto make_permissions(std::filesystem::perms permissions) -> std::string {
    auto lambda = [permissions](auto x, auto y) {
        return (permissions & x) == std::filesystem::perms::none ? '-' : y;
    };

    return {
        lambda(std::filesystem::perms::owner_read, 'r'),
        lambda(std::filesystem::perms::owner_write, 'w'),
        lambda(std::filesystem::perms::owner_exec, 'x'),
    };
}

auto size(std::filesystem::path const& path) {
    auto size = 0uz;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (!std::filesystem::is_directory(entry.status())) {
                size += std::filesystem::file_size(entry);
            }
        }
    }

    return size;
}

auto size(std::filesystem::directory_entry const& entry) {
    auto size = 0uz;

    if (std::filesystem::is_regular_file(entry.status())) {
        size = std::filesystem::file_size(entry);
    } else {
        size = ::size(entry.path());
    }

    std::vector<char> vector = {'B', 'K', 'M', 'G'};

    auto i = 0uz;

    while (i < std::size(vector) && size >= 1 << 10) {
        size /= (1 << 10);
        ++i;
    }

    return (std::stringstream() << std::format("{: >4} ({})", size, vector[i])).str();
}

void show(std::filesystem::path const& path, std::regex const& pattern) {
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        for (auto const& entry : std::filesystem::directory_iterator(path)) {
            auto name = entry.path().filename().string();

            if (!std::regex_search(name, pattern)) continue;

            std::print("show : entry : {} | {} | {} | {} | {}\n", make_type(entry.status()),
                       make_permissions(entry.status().permissions()), size(entry),
                       std::chrono::floor<std::chrono::seconds>(
                           std::chrono::file_clock::to_sys(entry.last_write_time())),
                       name);
        }
    }
}

int main(int argc, char** argv) {
    CLI::App app{"directory_filter"};

    std::string path = std::filesystem::current_path().string();
    std::string pattern;

    app.add_option("path", path, "Directory to search")->check(CLI::ExistingDirectory);
    app.add_option("pattern", pattern, "Regex pattern to match filenames")->required();

    CLI11_PARSE(app, argc, argv);

    show(path, std::regex(pattern));
}
