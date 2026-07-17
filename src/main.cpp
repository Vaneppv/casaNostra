#include "family_tree/family_tree.hpp"
#include "presentation/main_menu.hpp"

int main() {
    FamilyTree tree;
    MainMenu menu("Casa Nostra - Árbol Binario", &tree);

    menu.show_menu();
    return 0;
}
