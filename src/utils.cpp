#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <array>
#include <optional>
#include <unistd.h>
#include <filesystem>

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

bool set_working_dir(const std::string& path) {
    std::string expanded_path = expand_tilde(path);
    const auto base_path = std::filesystem::path{expanded_path};
    try {
        std::filesystem::current_path(base_path);
    }
    catch (std::filesystem::filesystem_error& error) {
        std::cerr << "Error changing directory: " << error.what() << std::endl;
        return false;
    }

    return true;
}

bool create_dir(const std::string& path) {
    try {
        const auto dir_path = std::filesystem::path{expand_tilde(path)};

        std::filesystem::create_directory(dir_path);
    }
    catch (std::filesystem::filesystem_error& error) {
        std::cerr << "Error creating dir: " << error.what() << std::endl;
        return false;
    }
    return true;
}

bool move_file(const std::string& src, const std::string& dest) {
    try {
        std::filesystem::rename(src, dest);
    }
    catch (std::filesystem::filesystem_error &e) {
        std::cerr << "Error moving file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool copy_file(const std::string& src, const std::string& dest) {
    try {
        std::filesystem::copy(src, dest);
    }
    catch (std::filesystem::filesystem_error &e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return false;
    }
    return true;
}
