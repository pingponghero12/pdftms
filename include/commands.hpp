#pragma once

#include <string>
#include <vector>

#define CONFIG_PATH "~/.config/pdftms.yaml"
// #define CONFIG_PATH "~/Documents/pdftms/config.yaml"

// Default command
// IO () -> IO (Int)
int view_default(void);

// IO ([String]) -> IO (Int)
int create(const std::vector<std::string>& args);
int mv(const std::vector<std::string>& args);
int add(const std::vector<std::string>& args);
int mkdir(const std::vector<std::string>& args);
int rename_cmd(const std::vector<std::string>& args);
