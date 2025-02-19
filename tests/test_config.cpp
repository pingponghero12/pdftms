#include <gtest/gtest.h>
#include "config.hpp"
#include <filesystem>
#include <fstream>

class ConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
        original_home = std::getenv("HOME");

        test_home_ = std::filesystem::temp_directory_path() / "pdftms_test_home";
        std::filesystem::create_directories(test_home_);
        setenv("HOME", test_home_.c_str(), 1);

        config_dir_ = test_home_ / ".config";
        std::filesystem::create_directories(config_dir_);

        test_config.base_dir_path = "/test/documents";
        test_config.pdf_viewer = "test-viewer";
    }

    void TearDown() override {
        if (original_home) {
            setenv("HOME", original_home, 1);
        } else {
            unsetenv("HOME");
        }

        std::filesystem::remove_all(test_home_);
    }

    const char* original_home;
    std::filesystem::path test_home_;
    std::filesystem::path config_dir_;
    Config test_config;

    void write_yaml_to_file(const std::string& path, const std::string& content) {
        std::ofstream file(path);
        file << content;
    }

    std::string read_file(const std::string& path) {
        std::ifstream file(path);
        return std::string(std::istreambuf_iterator<char>(file),
                          std::istreambuf_iterator<char>());
    }
};

TEST_F(ConfigTest, YamlEncode) {
    YAML::Node node = YAML::convert<Config>::encode(test_config);

    EXPECT_EQ(node["base_dir_path"].as<std::string>(), "/test/documents");
    EXPECT_EQ(node["pdf_viewer"].as<std::string>(), "test-viewer");
}

TEST_F(ConfigTest, YamlDecode) {
    YAML::Node node;
    node["base_dir_path"] = "/test/documents";
    node["pdf_viewer"] = "test-viewer";

    Config config;
    bool result = YAML::convert<Config>::decode(node, config);

    EXPECT_TRUE(result);
    EXPECT_EQ(config.base_dir_path, "/test/documents");
    EXPECT_EQ(config.pdf_viewer, "test-viewer");
}

TEST_F(ConfigTest, YamlDecodeInvalidNode) {
    YAML::Node node = YAML::Load("- invalid\n- sequence");
    Config config;

    bool result = YAML::convert<Config>::decode(node, config);
    EXPECT_FALSE(result);
}

TEST_F(ConfigTest, CreateConfig) {
    create_config(test_config);

    std::filesystem::path config_path = config_dir_ / "pdftms.yaml";
    ASSERT_TRUE(std::filesystem::exists(config_path));

    std::string content = read_file(config_path);
    YAML::Node node = YAML::Load(content);

    EXPECT_EQ(node["base_dir_path"].as<std::string>(), "/test/documents");
    EXPECT_EQ(node["pdf_viewer"].as<std::string>(), "test-viewer");
}

TEST_F(ConfigTest, ReadConfig) {
    std::string yaml_content = 
        "base_dir_path: /test/documents\n"
        "pdf_viewer: test-viewer\n";

    std::filesystem::path config_path = config_dir_ / "test_config.yaml";
    write_yaml_to_file(config_path, yaml_content);

    Config config = read_config(config_path);

    EXPECT_EQ(config.base_dir_path, "/test/documents");
    EXPECT_EQ(config.pdf_viewer, "test-viewer");
}

TEST_F(ConfigTest, ReadNonExistentConfig) {
    std::filesystem::path non_existent_path = config_dir_ / "nonexistent.yaml";

    EXPECT_THROW(read_config(non_existent_path), YAML::BadFile);
}

TEST_F(ConfigTest, ReadInvalidConfig) {
    std::string invalid_yaml = "invalid: yaml: content: - [";
    std::filesystem::path invalid_config_path = config_dir_ / "invalid.yaml";
    write_yaml_to_file(invalid_config_path, invalid_yaml);

    EXPECT_THROW(read_config(invalid_config_path), YAML::ParserException);
}

TEST_F(ConfigTest, ReadConfigMissingFields) {
    std::string incomplete_yaml = "base_dir_path: /test/documents\n";
    std::filesystem::path incomplete_config_path = config_dir_ / "incomplete.yaml";
    write_yaml_to_file(incomplete_config_path, incomplete_yaml);

    EXPECT_THROW(read_config(incomplete_config_path), YAML::InvalidNode);
}
