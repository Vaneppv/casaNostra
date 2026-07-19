#include <iostream>
#include <stdexcept>

#include "family_tree/family_tree.hpp"
#include "presentation/main_menu.hpp"

bool load_csv_file(FamilyTree& tree, const std::string& path) {
    if (path.empty()) {
        return false;
    }
    std::cout << "Cargando datos...\n";
    return tree.load_from_csv(path);
}

int main() {
    FamilyTree tree;
    MainMenu menu("Casa Nostra - Árbol Binario", &tree);

    menu.set_option("Cargar datos desde CSV", [&menu, &tree]() {
        std::string path = menu.prompt_input("Ingrese ruta del archivo CSV: ");
        if (path.empty()) {
            path = "bin/datos.csv";
        }
        if (load_csv_file(tree, path)) {
            menu.print_success("Datos cargados correctamente.");
        } else {
            menu.print_error("No se pudo cargar el archivo.");
        }
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
