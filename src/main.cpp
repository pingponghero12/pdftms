#include <iostream>
#include <string>
#include <unistd.h>
#include <optional>
#include <functional>
#include <unordered_map>

#include "config.hpp"
#include "utils.hpp"

// #define CONFIG_PATH "~/.config/pdftms.config"
#define CONFIG_PATH "~/Documents/pdftms/config.yaml"

using CommandFunction = std::function<int(const std::vector<std::string>&)>;

int view_default(const std::vector<std::string>&) {
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


int main(int argc, char* argv[]) {
    std::unordered_map<std::string, CommandFunction> commands;

    std::vector<std::string> args(argv+1, argv + argc);
    std::string command;

    if (!args.empty()) {
        command = args.at(0);
    }
    else {
        return view_default(args);
    }

    if (commands.find(command) != commands.end()) {
        return commands[command](args);
    }
    else {
        std::cerr << "Command not found" << std::endl;
        return EXIT_FAILURE;
    }
}
