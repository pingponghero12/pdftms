#include "fs.hpp"
#include "utils.hpp"
#include "config.hpp"

#include <iostream>
#include <optional>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

bool enter_vault(void) {
    // Filesystem and YAML need expanded tilde
    const std::string config_path = expand_tilde(CONFIG_PATH);
    const Config config = read_config(config_path);

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
    const std::string config_path = expand_tilde(CONFIG_PATH);
    const Config config = read_config(config_path);

    return config.pdf_viewer;
}

std::optional<fs::path> get_dest_path(const std::string& dest_dir_str, const std::string& pdf_name) {
    const bool is_dir = true;
    const auto dest_dir = get_dest(dest_dir_str, is_dir);
    if (!dest_dir.has_value()) {
        return {};
    }
    const fs::path pdf_path(pdf_name);
    const fs::path dest_path = dest_dir.value() / pdf_path.filename();

    return dest_path;
}

std::optional<fs::path> get_dest(const std::string& dest_str,const bool& is_dir) {
    // We avoid const here for cannonical, possible to make this pure
    fs::path dest(dest_str);
    if (!dest.is_absolute()) {
        dest = fs::absolute(dest);
    }

    try {
        dest = fs::canonical(dest);
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error resolving destination: " << e.what() << std::endl;
        return {};
    }

    if (!fs::exists(dest)) {
        std::cerr << "Destination does not exist: " << dest << std::endl;
        return {};
    }

    if (is_dir && !fs::is_directory(dest)) {
        std::cerr << "Destination is not a directory: " << dest << std::endl;
        return {};
    }

    return dest;
}

bool set_working_dir(const std::string& path) {
    const std::string config_path = expand_tilde(CONFIG_PATH);
    const Config config = read_config(config_path);

    const std::string expanded_path = expand_tilde(path);
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

std::optional<std::tuple<std::string, std::string>> new_insert_helper(const std::string& file_input) {
    const std::optional<fs::path> src_path = get_src_path(file_input);
    if (!src_path.has_value()) {
        return {};
    }

    if (!enter_vault()) {
        return {};
    }

    const auto dest = fzf_dir();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return {};
    }

    const std::string dest_dir_str = rtrim(expand_tilde(dest.value()));

    // Not const coz of cin
    std::string pdf_name;
    std::cout << "New PDF name(<name>.pdf): ";
    std::cin >> pdf_name;

    const std::optional<fs::path> dest_path = get_dest_path(dest_dir_str, pdf_name);
    if (!dest_path.has_value()) {
        return {};
    }

    return make_tuple(src_path.value().string(), dest_path.value().string());
}
