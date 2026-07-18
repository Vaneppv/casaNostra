#pragma once

#include <array>
#include <functional>
#include <string>

#include "../family_tree/family_tree.hpp"

struct MenuOption {
    const char* description;
    std::function<void()> action;
};

class MainMenu {
private:
    static constexpr int MAX_OPTIONS = 10;
    std::string m_title;
    int m_num_options {0};
    std::string m_exit_text;
    std::array<MenuOption, MAX_OPTIONS> m_options;

    FamilyTree* m_tree;

public:
    // move() safely moves title ownership to this class' instance
    MainMenu(std::string title, FamilyTree* tree);
    ~MainMenu() = default;

    void show_menu();
    bool confirm_action(const std::string& prompt);
    std::string prompt_input(const std::string& prompt);
    void print_success(const std::string& message) const;
    void print_error(const std::string& error) const;
    void set_title(const std::string& title);
    std::string get_title() const { return m_title; }
    void set_exit_text(const std::string& text);
    void set_option(const char* desc, std::function<void()> action);
};
