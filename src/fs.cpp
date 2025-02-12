#include "fs.hpp"
#include "utils.hpp"
#include "config.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <array>
#include <optional>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

bool enter_vault(void) {
    // Filesystem and YAML need expanded tilde
    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);

    if (!set_working_dir(config.base_dir_path)) {
        return false;
    }
    return true;
}

std::optional<fs::path> get_src_path(const std::string& file_input) {
    fs::path local_path = fs::current_path();
    fs::path src_path(file_input);
    src_path = local_path / src_path;

    if (!fs::exists(src_path)) {
        std::cerr << "Source file does not exist: " << src_path << std::endl;
        return {};
    }
    return src_path;
}

std::string get_viewer(void) {
    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);

    return config.pdf_viewer;
}

std::optional<fs::path> get_dest_path(const std::string& dest_dir_str, const std::string& pdf_name) {
    fs::path dest_dir(dest_dir_str);
    if (!dest_dir.is_absolute()) {
        dest_dir = fs::absolute(dest_dir);
    }

    try {
        dest_dir = fs::canonical(dest_dir);
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error resolving destination directory: " << e.what() << std::endl;
        return {};
    }

    if (!fs::exists(dest_dir) || !fs::is_directory(dest_dir)) {
        std::cerr << "Destination is not a valid directory: " << dest_dir << std::endl;
        return {};
    }
    fs::path pdf_path(pdf_name);
    fs::path dest_path = dest_dir / pdf_path.filename();

    return dest_path;
}

bool set_working_dir(const std::string& path) {
    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);

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

std::optional<std::tuple<std::string, std::string>> insert_base(const std::string& file_input) {
    fs::path local_path = fs::current_path();
    fs::path src_path(file_input);
    src_path = local_path / src_path;

    if (!fs::exists(src_path)) {
        std::cerr << "Source file does not exist: " << src_path << std::endl;
        return {};
    }

    if (!enter_vault()) {
        return {};
    }

    auto dest = fzf_dir();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return {};
    }

    std::string dest_dir_str = rtrim(expand_tilde(dest.value()));
    fs::path dest_dir(dest_dir_str);
    if (!dest_dir.is_absolute()) {
        dest_dir = fs::absolute(dest_dir);
    }

    try {
        dest_dir = fs::canonical(dest_dir);
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error resolving destination directory: " << e.what() << std::endl;
        return {};
    }

    if (!fs::exists(dest_dir) || !fs::is_directory(dest_dir)) {
        std::cerr << "Destination is not a valid directory: " << dest_dir << std::endl;
        return {};
    }

    std::string pdf_name;

    std::cout << "New PDF name(<name>.pdf): ";
    std::cin >> pdf_name;

    fs::path pdf_path(pdf_name);
    fs::path dest_path = dest_dir / pdf_path.filename();

    return make_tuple(src_path.string(), dest_path.string());
}
