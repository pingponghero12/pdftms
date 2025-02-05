#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

struct Config {
    std::string base_dir_path;
    std::string pdf_viewer;
};

namespace YAML {
    template<>
    struct convert<Config> {
        static Node encode(const Config& config);
        static bool decode(const Node& node, Config& config);
    };
}

void create_config(const Config& config);
Config read_config(const std::string& config_path);
