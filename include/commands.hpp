#pragma once

#include <string>
#include <vector>

// Default command
// IO () -> IO (Int)
int view_default(void);

// IO ([String]) -> IO (Int)
int create(const std::vector<std::string>& args);
int mv(const std::vector<std::string>& args);
int add(const std::vector<std::string>& args);
int mkdir(const std::vector<std::string>& args);
int rename_cmd(const std::vector<std::string>& args);
