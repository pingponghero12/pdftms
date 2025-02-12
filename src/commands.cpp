#include "commands.hpp"
#include "config.hpp"
#include "utils.hpp"
#include "fs.hpp"

#include <iostream>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

int view_default(void) {
    if (!enter_vault()) {
        return EXIT_FAILURE;
    }

    std::optional<std::string> file = fzf();
    if (!file.has_value()) {
        return EXIT_SUCCESS;
    }

    std::string pdf_viewer = get_viewer();
    execlp(pdf_viewer.c_str(), pdf_viewer.c_str(), rtrim(file.value()).c_str(), NULL);

    std::cerr << "Failed to exec " << pdf_viewer << std::endl;
    return EXIT_FAILURE;
}

int create(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        std::cerr << "Usage: pdftms create <path> <pdf_reader>\n"
            << "<path> needs to point to valid existing or new directory"
            << "<pdf_reader> command that runs pdf_viewer of your choise"
            << std::endl;
        return EXIT_FAILURE;
    }

    const std::string dir_path = args.at(1);
    const std::string pdf_viewer = args.at(2);

    if (!create_dir(dir_path)) {
        return EXIT_FAILURE;
    }

    Config config = {dir_path, pdf_viewer};
    create_config(config);

    return EXIT_SUCCESS;
}

int mv(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cerr << "Usage: pdftms mv <file>\n"
            << "<path> needs to point to valid PDF file"
            << std::endl;
        return EXIT_FAILURE;
    }
    const std::string file_input = args.at(1);

    std::optional<fs::path> src_path = get_src_path(file_input);
    if (!src_path.has_value()) {
        return EXIT_FAILURE;
    }

    if (!enter_vault()) {
        return EXIT_FAILURE;
    }

    auto dest = fzf_dir();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return EXIT_FAILURE;
    }

    std::string dest_dir_str = rtrim(expand_tilde(dest.value()));

    std::string pdf_name;
    std::cout << "New PDF name(<name>.pdf): ";
    std::cin >> pdf_name;

    std::optional<fs::path> dest_path = get_dest_path(dest_dir_str, pdf_name);
    if (!dest_path.has_value()) {
        return EXIT_FAILURE;
    }

    if (!move_file(src_path.value().string(), dest_path.value().string())) {
        std::cerr << "Failed to move file to: " << dest_path.value().string() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int add(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cerr << "Usage: pdftms add <file>\n"
            << "<path> needs to point to valid PDF file\n"
            << "Add does not move file, but copies it to pdf vault."
            << std::endl;
        return EXIT_FAILURE;
    }
    const std::string file_input = args.at(1);

    auto paths = insert_base(file_input);
    std::string src_path = get<0>(paths);
    std::string dest_path = get<1>(paths);

    if (!copy_file(src_path.string(), dest_path.string())) {
        std::cerr << "Failed to move file to: " << dest_path << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int mkdir(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "Usage: pdftms mkdir"
            << std::endl;
        return EXIT_FAILURE;
    }

    if (!enter_vault()) {
        return EXIT_FAILURE;
    }

    auto dest = fzf_dir();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    if (!fs::exists(dest_dir) || !fs::is_directory(dest_dir)) {
        std::cerr << "Destination is not a valid directory: " << dest_dir << std::endl;
        return EXIT_FAILURE;
    }

    std::string new_dir_name;
    std::cout << "Enter dir name: ";
    std::cin >> new_dir_name;
    fs::path new_dir(new_dir_name);

    dest_dir = dest_dir / new_dir;

    try {
        fs::create_directory(dest_dir);
    }
    catch (fs::filesystem_error& e) {
        std::cerr << "Error creating dir: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int rename_cmd(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cerr << "Usage: pdftms rename\n"
            << std::endl;
        return EXIT_FAILURE;
    }

    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);

    if (!set_working_dir(config.base_dir_path)) {
        return EXIT_FAILURE;
    }

    auto dest = fzf();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return EXIT_FAILURE;
    }

    std::string dest_str = rtrim(expand_tilde(dest.value()));
    fs::path dest_path(dest_str);
    if (!dest_path.is_absolute()) {
        dest_path = fs::absolute(dest_path);
    }

    try {
        dest_path = fs::canonical(dest_path);
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error resolving destination directory: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (!fs::exists(dest_path)) {
        std::cerr << "Destination does not exists: " << dest_path << std::endl;
        return EXIT_FAILURE;
    }

    std::string pdf_name;

    std::cout << "Old name: " << dest_path.filename().string() << std::endl;
    std::cout << "New PDF name(<name>.pdf): ";
    std::cin >> pdf_name;

    fs::path pdf_path(pdf_name);

    fs::path new_path = dest_path.parent_path() / pdf_path.filename();

    if (!move_file(dest_path.string(), new_path.string())) {
        std::cerr << "Failed to rename file to: " << dest_path << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
