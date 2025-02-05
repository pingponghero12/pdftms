#include <iostream>
#include <string>
#include <unistd.h>
#include <optional>

#include "config.hpp"
#include "utils.hpp"

// #define CONFIG_PATH "~/.config/pdftms.config"
#define CONFIG_PATH "~/Documents/pdftms/config.yaml"

int main(void) {
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
