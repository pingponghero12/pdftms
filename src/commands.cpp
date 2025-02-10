#include "commands.hpp"
#include "config.hpp"
#include "utils.hpp"

#include <iostream>
#include <optional>

int view_default(void) {
    // Filesystem and YAML need expanded tilde
    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);

    if (!set_working_dir(config.base_dir_path)) {
        return EXIT_FAILURE;
    }

    std::optional<std::string> file = fzf();
    if (!file.has_value()) {
        return EXIT_SUCCESS;
    }

    auto pdf_viewer = config.pdf_viewer.c_str();
    execlp(pdf_viewer, pdf_viewer, rtrim(file.value()).c_str(), NULL);

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
