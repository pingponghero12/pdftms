#pragma once

#include <string>
#include <vector>

#define CONFIG_PATH "~/.config/pdftms.yaml"
// #define CONFIG_PATH "~/Documents/pdftms/config.yaml"

int view_default(void);
int create(const std::vector<std::string>& args);
int add(const std::vector<std::string>& args);
int mkdir(const std::vector<std::string>& args);

