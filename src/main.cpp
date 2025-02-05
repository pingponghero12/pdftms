#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <memory>
#include <optional>
#include <array>
#include <filesystem>
#include <unistd.h>

#include "config.hpp"
#include "utils.hpp"

//#define CONFIG_PATH "~/.config/pdftms.config"
#define CONFIG_PATH "~/Documents/pdftms/config.yaml"

// IO () -> IO (Maybe String)
std::optional<std::string> fzf() {
    std::array<char, 256> buffer;
    std::string output;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("fzf", "r"), pclose);

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

int main(void) {
    // Filesystem and YAML need expanded tilde
    std::string config_path = expand_tilde(CONFIG_PATH);
    Config config = read_config(config_path);
    config.base_dir_path = expand_tilde(config.base_dir_path);

    const auto base_path = std::filesystem::path{config.base_dir_path};
    try {
        std::filesystem::current_path(base_path);
    }
    catch (std::filesystem::filesystem_error& Error) {
        std::cerr << "filesystem error" << std::endl;
        return 0;
    }

    std::optional<std::string> file = fzf();
    if (!file.has_value()) {
        return 0;
    }

    auto pdfv = config.pdf_viewer.c_str();
    execlp(pdfv, pdfv, rtrim(file.value()).c_str(), NULL);
    return 0;
}
