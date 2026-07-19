#include <iostream>
#include <stdexcept>

#include "family_tree/family_tree.hpp"
#include "presentation/main_menu.hpp"

int main() {
    FamilyTree tree;
    MainMenu menu("Casa Nostra - Árbol Binario", &tree);

    menu.set_option("Cargar datos desde CSV", [&menu, &tree]() {
        std::string path = menu.prompt_input("Ingrese ruta del archivo CSV: ");

        std::cout << "Cargando datos...\n";
        tree.load_from_csv(path.empty() ? "bin/datos.csv" : path);
        menu.print_success("Datos cargados correctamente.");
    });

    menu.set_option("Editar miembro", [&menu, &tree]() {
        std::string input = menu.prompt_input("Ingrese ID del miembro: ");

        if (input.empty()) {
            menu.print_error("ID no válido.");
            return;
        }

        try {
            size_t pos;
            int member_id = std::stoi(input, &pos);
            if (pos != input.length()) {
                menu.print_error("ID debe ser un número entero.");
                return;
            }
            tree.edit_member(member_id);
        } catch (const std::invalid_argument&) {
            menu.print_error("ID debe ser un número entero.");
        } catch (const std::out_of_range&) {
            menu.print_error("ID fuera de rango.");
        }
    });

    menu.set_option("Ver línea de sucesión", [&tree]() { tree.show_succession(); });

    menu.set_option("Reasignar jefe automáticamente", [&tree]() { tree.check_and_assign_boss(); });

    menu.show_menu();

    return 0;
}
