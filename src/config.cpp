#include "config.hpp"
#include <fstream>

namespace YAML {
    Node convert<Config>::encode(const Config& config) {
        Node node;
        node["base_dir_path"] = config.base_dir_path;
        node["pdf_viewer"] = config.pdf_viewer;

        return node;
    }

    bool convert<Config>::decode(const Node& node, Config& config) {
        if (!node.IsMap()) return false;

        config.base_dir_path = node["base_dir_path"].as<std::string>();
        config.pdf_viewer = node["pdf_viewer"].as<std::string>();

        return true;
    }
}

void create_config(const Config& config) {
    YAML::Node node = YAML::convert<Config>::encode(config);

    std::ofstream fout("config.yaml");
    fout << node;
}

Config read_config(const std::string& config_path) {
    YAML::Node node = YAML::LoadFile(config_path);
    Config config = node.as<Config>();

    return config;
}

