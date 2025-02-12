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
