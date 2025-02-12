#pragma once

#include <string>

// String -> IO Bool
bool set_working_dir(const std::string& path);

// String -> IO Bool
bool create_dir(const std::string& path);

bool move_file(const std::string& src, const std::string& dest);
bool copy_file(const std::string& src, const std::string& dest);
