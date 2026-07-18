#include "main_menu.hpp"

#include <array>
#include <cctype>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "../utils/constants.hpp"

using namespace Constants::ASCII_CODES;

MainMenu::MainMenu(std::string title, FamilyTree* tree) : m_title(std::move(title)), m_tree(tree) {
    set_exit_text("Salir");

    set_option("Cargar datos desde CSV", [this]() {
        m_tree->load_from_csv("bin/datos.csv");
        print_success("Datos cargados correctamente.");
    });

    set_option("Editar miembro", [this]() {
        std::string input = prompt_input("Ingrese ID del miembro: ");

        if (input.empty()) {
            print_error("ID no válido.");
            return;
        }

        try {
            size_t pos;
            int member_id = std::stoi(input, &pos);
            if (pos != input.length()) {
                print_error("ID debe ser un número entero.");
                return;
            }
            m_tree->edit_member(member_id);
        } catch (const std::invalid_argument&) {
            print_error("ID debe ser un número entero.");
        } catch (const std::out_of_range&) {
            print_error("ID fuera de rango.");
        }
    });

    set_option("Ver línea de sucesión", [this]() { m_tree->show_succession(); });

    set_option("Reasignar jefe automáticamente", [this]() {
        m_tree->check_and_assign_boss();
    });
}

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

void MainMenu::set_option(const char* desc, std::function<void()> action) {
    if (m_num_options < MAX_OPTIONS) {
        m_options[m_num_options].description = desc;
        m_options[m_num_options].action = action;
        m_num_options++;
    }
}

bool MainMenu::confirm_action(const std::string& prompt) {
    std::cout << COLOR_YELLOW << prompt << COLOR_RESET;
    char confirm = 'n';
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return confirm == 's' || confirm == 'S';
}

std::string MainMenu::prompt_input(const std::string& prompt) {
    std::cout << COLOR_YELLOW << prompt << COLOR_RESET;
    std::string value;
    std::getline(std::cin >> std::ws, value);
    return value;
}

void MainMenu::show_menu() {
    if (m_num_options <= 0) {
        return;
    }

    int option = -1;
    do {
        std::cout << COLOR_CYAN << "\n=== " << m_title << " ===" << COLOR_RESET << "\n";
        for (int i = 0; i < m_num_options; ++i) {
            std::cout << COLOR_YELLOW << i + 1 << "." << COLOR_RESET << " "
                      << m_options[i].description << "\n";
        }
        std::cout << COLOR_RED << "0." << COLOR_RESET << " " << m_exit_text << "\n";
        std::cout << COLOR_YELLOW << "Seleccione una opción: ";

        if (!(std::cin >> option) || std::cin.peek() != '\n') {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << COLOR_RED << "Opción inválida. Debe ser numérica." << COLOR_RESET << "\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (option < 0 || option > m_num_options) {
            std::cout << COLOR_RED << "Opción inválida" << COLOR_RESET << "\n";
            continue;
        }

        int index = option - 1;
        if (index >= 0 && index < m_num_options && m_options[index].action != nullptr) {
            m_options[index].action();
        }

        // Si el indice es -1 ((index = 0) -1) se sale del menu
        if (index == -1) {
            std::cout << COLOR_GREEN
                      << (tolower(m_exit_text[0]) == 's' ? "Saliendo..." : "Volviendo...")
                      << COLOR_RESET << "\n";
            break;
        }
    } while (option != 0);
}
