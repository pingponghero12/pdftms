#include <iostream>
#include <string>
#include <unistd.h>
#include <optional>
#include <filesystem>

#include "config.hpp"
#include "utils.hpp"

// #define CONFIG_PATH "~/.config/pdftms.config"
#define CONFIG_PATH "~/Documents/pdftms/config.yaml"

int main(void) {
    // Filesystem and YAML need expanded tilde
    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);

    const auto base_path = std::filesystem::path{expand_tilde(config.base_dir_path)};
    try {
        std::filesystem::current_path(base_path);
    }
    catch (std::filesystem::filesystem_error& error) {
        std::cerr << "filesystem error: " << error.what() << std::endl;
        return 0;
    }

    std::optional<std::string> file = fzf();
    if (!file.has_value()) {
        return 0;
    }

    auto pdf_viewer = config.pdf_viewer.c_str();
    execlp(pdf_viewer, pdf_viewer, rtrim(file.value()).c_str(), NULL);

    std::cerr << "Failed to exec " << pdf_viewer << std::endl;
    return EXIT_FAILURE;
}
