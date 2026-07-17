#pragma once
#include <string_view>

namespace Constants::ASCII_CODES {
constexpr std::string_view CLEAR_SCREEN = "\033[2J\033[H";
constexpr std::string_view COLOR_RESET = "\033[0m";
constexpr std::string_view COLOR_RED = "\033[31m";
constexpr std::string_view COLOR_GREEN = "\033[32m";
constexpr std::string_view COLOR_YELLOW = "\033[33m";
constexpr std::string_view COLOR_CYAN = "\033[36m";
} // namespace Constants::ASCII_CODES
