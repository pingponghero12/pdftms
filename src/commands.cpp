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

    const std::string pdf_viewer = get_viewer();
    execlp(pdf_viewer.c_str(), pdf_viewer.c_str(), rtrim(file.value()).c_str(), NULL);

    // execlp should overwrite this program
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

    const Config config = {dir_path, pdf_viewer};
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

    const auto paths = new_insert_helper(file_input);
    if (!paths.has_value()) {
        return EXIT_FAILURE;
    }
    const std::string src_path = get<0>(paths.value());
    const std::string dest_path = get<1>(paths.value());

    if (!move_file(src_path, dest_path)) {
        std::cerr << "Failed to move file to: " << dest_path << std::endl;
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

    const auto paths = new_insert_helper(file_input);
    if (!paths.has_value()) {
        return EXIT_FAILURE;
    }
    const std::string src_path = get<0>(paths.value());
    const std::string dest_path = get<1>(paths.value());

    if (!copy_file(src_path, dest_path)) {
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

    const auto dest = fzf_dir();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return EXIT_FAILURE;
    }

    const std::string dest_dir_str = rtrim(expand_tilde(dest.value()));
    const bool is_dir = true;
    auto dest_dir = get_dest(dest_dir_str, is_dir);
    if (!dest_dir.has_value()) {
        return EXIT_FAILURE;
    }

    std::string new_dir_name;
    std::cout << "Enter dir name: ";
    std::cin >> new_dir_name;

    const fs::path new_dir(new_dir_name);
    const fs::path out_dir = dest_dir.value() / new_dir;

    try {
        fs::create_directory(out_dir);
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

    if (!enter_vault()) {
        return {};
    }

    auto dest = fzf();
    if (!dest.has_value()) {
        std::cerr << "No destination chosen." << std::endl;
        return EXIT_FAILURE;
    }

    const std::string dest_str = rtrim(expand_tilde(dest.value()));

    const bool is_dir = false;
    auto dest_path = get_dest(dest_str, is_dir);
    if (!dest_path.has_value()) {
        return EXIT_FAILURE;
    }

    std::string pdf_name;

    std::cout << "Old name: " << dest_path.value().filename().string() << std::endl;
    std::cout << "New PDF name(<name>.pdf): ";
    std::cin >> pdf_name;

    fs::path pdf_path(pdf_name);

    fs::path new_path = dest_path.value().parent_path() / pdf_path.filename();

    if (!move_file(dest_path.value().string(), new_path.string())) {
        std::cerr << "Failed to rename file to: " << dest_path.value().string() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
