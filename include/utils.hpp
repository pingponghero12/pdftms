#pragma once

#include <string>
#include <optional>

// String -> IO String
std::string expand_tilde(const std::string& path);
// String -> String
std::string rtrim(const std::string& s);

// IO () -> IO (Maybe String)
std::optional<std::string> fzf();
std::optional<std::string> fzf_dir();
// String -> IO Bool
bool set_working_dir(const std::string& path);

// String -> IO Bool
bool create_dir(const std::string& path);

bool move_file(const std::string& src, const std::string& dest);
