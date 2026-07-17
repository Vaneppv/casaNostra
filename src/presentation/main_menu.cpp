#include "main_menu.hpp"

#include <iostream>
#include <limits>

#include "constants.hpp"

using namespace Constants::ASCII_CODES;

void MainMenu::print_error(const std::string& error) const {
    std::cout << COLOR_RED << error << COLOR_RESET << "\n";
}

void MainMenu::print_success(const std::string& message) const {
    std::cout << COLOR_GREEN << message << COLOR_RESET << "\n";
}

void MainMenu::set_title(const std::string& title) {
    if (title.empty()) {
        return;
    }

    this->m_title = title;
}

void MainMenu::set_exit_text(const std::string& text) {
    if (text.empty()) {
        return;
    }

    this->m_exit_text = text;
}

void MainMenu::set_option(int index, const char* desc, std::function<void()> action) {
    if (index >= 0 && index < MainMenu::MAX_OPTIONS) {
        this->m_options[index].description = desc;
        this->m_options[index].action = action;
    }
}

bool MainMenu::confirm_action(const std::string& prompt) {
    std::cout << COLOR_YELLOW << prompt << COLOR_RESET;
    char confirm = 'n';
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return confirm == 's' || confirm == 'S';
}
