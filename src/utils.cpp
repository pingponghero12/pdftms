#include "utils.hpp"

#include <algorithm>
#include <memory>
#include <array>
#include <optional>
#include <unistd.h>

std::string expand_tilde(const std::string& path) {
    if (!path.empty() && path[0] == '~') {
        const char* home = std::getenv("HOME");
        if (home) {
            return std::string(home) + path.substr(1);
        }
    }
    return path;
}

std::string rtrim(const std::string& s) {
    std::string result = s;
    result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), result.end());
    return result;
}

std::optional<std::string> fzf() {
    std::array<char, 256> buffer;
    std::string output;

    // Custom deleter to avoid attribute ignore warnings
    auto close_file = [](FILE* file) {
        if (file) pclose(file);
    };
    std::unique_ptr<FILE, decltype(close_file)> pipe(popen("fzf", "r"), close_file);

    if(!pipe) {
        throw std::runtime_error("popen() failed");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        output += buffer.data();
    }

    if (output == "") {
        return {};
    }
    return output;
}

std::optional<std::string> fzf_dir() {
    std::array<char, 256> buffer;
    std::string output;

    // Custom deleter to avoid attribute ignore warnings
    auto close_file = [](FILE* file) {
        if (file) pclose(file);
    };
    std::unique_ptr<FILE, decltype(close_file)> pipe(popen("find . -type d | fzf", "r"), close_file);

    if(!pipe) {
        throw std::runtime_error("popen() failed");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        output += buffer.data();
    }

    if (output == "") {
        return {};
    }
    return output;
}

