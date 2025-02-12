#pragma once

#include <string>
#include <filesystem>
#include <optional>

#define CONFIG_PATH "~/.config/pdftms.yaml"
// #define CONFIG_PATH "~/Documents/pdftms/config.yaml"

// String -> IO Bool
bool set_working_dir(const std::string& path);

// String -> IO Bool
bool create_dir(const std::string& path);

bool move_file(const std::string& src, const std::string& dest);
bool copy_file(const std::string& src, const std::string& dest);

bool enter_vault(void);
std::string get_viewer(void);

std::optional<std::filesystem::path> get_dest_path(const std::string& dest_dir_str, const std::string& pdf_name);
