#include "fs.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <array>
#include <optional>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

bool set_working_dir(const std::string& path) {
    std::string expanded_path = expand_tilde(path);
    const auto base_path = fs::path{expanded_path};
    try {
        fs::current_path(base_path);
    }
    catch (fs::filesystem_error& error) {
        std::cerr << "Error changing directory: " << error.what() << std::endl;
        return false;
    }

    return true;
}

bool create_dir(const std::string& path) {
    try {
        const auto dir_path = fs::path{expand_tilde(path)};

        fs::create_directory(dir_path);
    }
    catch (fs::filesystem_error& error) {
        std::cerr << "Error creating dir: " << error.what() << std::endl;
        return false;
    }
    return true;
}

bool move_file(const std::string& src, const std::string& dest) {
    try {
        fs::rename(src, dest);
    }
    catch (fs::filesystem_error &e) {
        std::cerr << "Error moving file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool copy_file(const std::string& src, const std::string& dest) {
    try {
        fs::copy(src, dest);
    }
    catch (fs::filesystem_error &e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return false;
    }
    return true;
}
