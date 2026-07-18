#include "family_tree.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void FamilyTree::delete_tree(Member* node) {
    if (node == nullptr) return;
    delete_tree(node->m_left);
    delete_tree(node->m_right);
    delete node;
}

Member* FamilyTree::find_member_by_id_rec(Member* node, int id) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->m_id == id) {
        return node;
    }
    Member* found = find_member_by_id_rec(node->m_left, id);
    if (found != nullptr) {
        return found;
    }
    return find_member_by_id_rec(node->m_right, id);
}

Member* FamilyTree::find_member_by_id(int id) const {
    return find_member_by_id_rec(m_root, id);
}

Member* FamilyTree::find_first_alive_free(Member* node) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (!node->m_is_dead && !node->m_in_jail) {
        return node;
    }
    Member* found = find_first_alive_free(node->m_left);
    if (found != nullptr) {
        return found;
    }
    return find_first_alive_free(node->m_right);
}

Member* FamilyTree::find_first_alive_jailed(Member* node) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (!node->m_is_dead && node->m_in_jail) {
        return node;
    }
    Member* found = find_first_alive_jailed(node->m_left);
    if (found != nullptr) {
        return found;
    }
    return find_first_alive_jailed(node->m_right);
}

Member* FamilyTree::find_current_boss(Member* node) const {
    if (node == nullptr) { 
        return nullptr; 
    }
    if (node->m_is_boss) { 
        return node; 
    }
    Member* found = find_current_boss(node->m_left);
    if (found != nullptr) { 
        return found; 
    }
    return find_current_boss(node->m_right);
}

Member* FamilyTree::find_successor(Member* boss, bool search_free) const {
    if (boss == nullptr) { 
        return nullptr; 
    }

    Member* succ;

    if (search_free) {
        succ = find_first_alive_free(boss->m_left);
        if (succ == nullptr) {
            succ = find_first_alive_free(boss->m_right);
        }
    } else {
        succ = find_first_alive_jailed(boss->m_left);
        if (succ == nullptr) {
            succ = find_first_alive_jailed(boss->m_right);
        }
    }
    if (succ != nullptr) { return succ; }

    if (boss->m_boss != nullptr) {
        Member* partner = (boss->m_boss->m_left == boss)
                              ? boss->m_boss->m_right
                              : boss->m_boss->m_left;
        if (partner != nullptr) {
            if (search_free) {
                succ = find_first_alive_free(partner->m_left);
                if (succ == nullptr) {
                    succ = find_first_alive_free(partner->m_right);
                }
            } else {
                succ = find_first_alive_jailed(partner->m_left);
                if (succ == nullptr) {
                    succ = find_first_alive_jailed(partner->m_right);
                }
            }
            if (succ != nullptr) { return succ; }

            if (!partner->m_is_dead) {
                if (search_free && !partner->m_in_jail) {
                    return partner;
                }
                if (!search_free && partner->m_in_jail) {
                    return partner;
                }
            }
        }
    }

    if (boss->m_boss != nullptr && boss->m_boss->m_boss != nullptr) {
        Member* grand_boss = boss->m_boss->m_boss;
        Member* grand_partner = (grand_boss->m_left == boss->m_boss)
                                    ? grand_boss->m_right
                                    : grand_boss->m_left;
        if (grand_partner != nullptr) {
            if (search_free) {
                succ = find_first_alive_free(grand_partner->m_left);
                if (succ == nullptr) {
                    succ = find_first_alive_free(
                        grand_partner->m_right);
                }
            } else {
                succ = find_first_alive_jailed(
                    grand_partner->m_left);
                if (succ == nullptr) {
                    succ = find_first_alive_jailed(
                        grand_partner->m_right);
                }
            }
            if (succ != nullptr) { return succ; }

            if (!grand_partner->m_is_dead) {
                if (search_free && !grand_partner->m_in_jail) {
                    return grand_partner;
                }
                if (!search_free && grand_partner->m_in_jail) {
                    return grand_partner;
                }
            }
        }
    }

    return find_nearest_boss_with_two(boss, search_free);
}

Member* FamilyTree::find_nearest_boss_with_two(
    Member* boss, bool search_free) const {
    Member* current = boss->m_boss;
    while (current != nullptr) {
        bool left_ok = false;
        bool right_ok = false;

        if (current->m_left != nullptr
            && !current->m_left->m_is_dead) {
            if (search_free && !current->m_left->m_in_jail) {
                left_ok = true;
            }
            if (!search_free && current->m_left->m_in_jail) {
                left_ok = true;
            }
        }
        if (current->m_right != nullptr
            && !current->m_right->m_is_dead) {
            if (search_free && !current->m_right->m_in_jail) {
                right_ok = true;
            }
            if (!search_free && current->m_right->m_in_jail) {
                right_ok = true;
            }
        }

        if (left_ok && right_ok) {
            if (current->m_left != nullptr
                && !current->m_left->m_is_dead) {
                if (search_free
                    && !current->m_left->m_in_jail) {
                    return current->m_left;
                }
                if (!search_free
                    && current->m_left->m_in_jail) {
                    return current->m_left;
                }
            }
            if (current->m_right != nullptr
                && !current->m_right->m_is_dead) {
                if (search_free
                    && !current->m_right->m_in_jail) {
                    return current->m_right;
                }
                if (!search_free
                    && current->m_right->m_in_jail) {
                    return current->m_right;
                }
            }
        }
        current = current->m_boss;
    }
    return nullptr;
}

void FamilyTree::attach_orphans() {
    bool progress;
    do {
        progress = false;
        OrphanQueue temp; // cola para los que no se puedan enlazar aún
        while (!m_orphan_queue.empty()) {
            Member* m = m_orphan_queue.pop();
            Member* boss = find_member_by_id(m->m_id_boss);
            if (boss != nullptr) {
                m->m_boss = boss;
                if (boss->m_left == nullptr)
                    boss->m_left = m;
                else if (boss->m_right == nullptr)
                    boss->m_right = m;
                else {
                    cerr << "Jefe " << boss->m_id << " lleno al intentar agregar a " << m->m_id
                         << "\n";
                    delete m; // no podemos agregarlo, se descarta
                }
                progress = true;
            } else {
                temp.push(m); // vuelve a la cola temporal
            }
        }
        // Devolver los que quedaron a la cola original
        while (!temp.empty()) {
            m_orphan_queue.push(temp.pop());
        }
    } while (progress && !m_orphan_queue.empty());
}

void FamilyTree::load_from_csv(const string& filename) {
    delete_tree(m_root);
    m_root = nullptr;
    while (!m_orphan_queue.empty()) {
        m_orphan_queue.pop();
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir " << filename << "\n";
        return;
    }

    string line;
    getline(file, line); // cabecera

    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        int id, age, id_boss;
        string name, last_name;
        char gender;
        bool is_dead, in_jail, was_boss, is_boss;

        // id
        getline(ss, token, ',');
        id = stoi(token);
        // name
        getline(ss, token, ',');
        name = token;
        // last_name
        getline(ss, token, ',');
        last_name = token;
        // gender
        getline(ss, token, ',');
        gender = token[0];

        getline(ss, token, ',');
        age = stoi(token);
        // id_boss
        getline(ss, token, ',');
        id_boss = stoi(token);
        // is_dead
        getline(ss, token, ',');
        is_dead = (stoi(token) == 1);
        // in_jail
        getline(ss, token, ',');
        in_jail = (stoi(token) == 1);
        // was_boss
        getline(ss, token, ',');
        was_boss = (stoi(token) == 1);
        // is_boss
        getline(ss, token, ',');
        is_boss = (stoi(token) == 1);

        Member* new_member = new Member(id, name, last_name, gender, age, id_boss, is_dead, in_jail,
                                        was_boss, is_boss);

        if (new_member->m_id_boss == 0) {
            if (m_root == nullptr) {
                m_root = new_member;
            } else {
                cerr << "Varias raíces; se mantiene la primera.\n";
                delete new_member;
                continue;
            }
        } else {
            Member* boss = find_member_by_id(new_member->m_id_boss);
            if (boss != nullptr) {
                // enlazar al jefe
                new_member->m_boss = boss;
                if (boss->m_left == nullptr)
                    boss->m_left = new_member;
                else if (boss->m_right == nullptr)
                    boss->m_right = new_member;
                else {
                    cerr << "Jefe " << boss->m_id << " ya tiene dos hijos.\n";
                    delete new_member;
                }
            } else {
                m_orphan_queue.push(new_member);
            }
        }
    }
    file.close();

    attach_orphans();

    if (m_root == nullptr) {
        OrphanQueue temp;
        while (!m_orphan_queue.empty()) {
            Member* m = m_orphan_queue.pop();
            if (m->m_id_boss == 0 || find_member_by_id(m->m_id_boss) == nullptr) {
                m_root = m;
                cerr << "Raíz asignada automáticamente al miembro " << m_root->m_id << "\n";
                break;
            } else {
                temp.push(m);
            }
        }
        // devolvemos los no elegidos a la cola original
        while (!temp.empty()) {
            m_orphan_queue.push(temp.pop());
        }
    }

    while (!m_orphan_queue.empty()) {
        Member* m = m_orphan_queue.pop();
        cerr << "Huérfano sin jefe: " << m->m_id << " (jefe " << m->m_id_boss
             << " no encontrado)\n";
        delete m; // Se liberan al no poder almacenarse
    }
}

void FamilyTree::check_and_assign_boss() {
    Member* boss = find_current_boss(m_root);
    if (boss == nullptr) {
        cout << "No hay un jefe actual\n";
        return;
    }

    if (!boss->m_is_dead && boss->m_age <= MAX_AGE
        && !boss->m_in_jail) {
        return;
    }

    cout << "El jefe " << boss->m_name << " " << boss->m_last_name
         << " necesita ser reemplazado";

    if (boss->m_is_dead) { cout << " (fallecido)"; }
    else if (boss->m_in_jail) { cout << " (en prision)"; }
    if (boss->m_age > MAX_AGE) {
        cout << " (mayor de " << MAX_AGE << " anios)";
    }
    cout << "\n";

    Member* candidate = find_successor(boss, true);
    if (candidate == nullptr) {
        candidate = find_successor(boss, false);
    }

    if (candidate == nullptr) {
        cout << "No se pudo encontrar un sucesor adecuado\n";
        return;
    }

    boss->m_is_boss = false;
    boss->m_was_boss = true;
    candidate->m_is_boss = true;

    cout << "Nuevo jefe asignado: " << candidate->m_name << " "
         << candidate->m_last_name
         << " (ID: " << candidate->m_id << ")\n";
}

void FamilyTree::edit_member(int id) {
    Member* member = find_member_by_id(id);
    if (member == nullptr) {
        cout << "No se encontro un miembro con ID " << id << "\n";
        return;
    }

    bool editing = true;
    while (editing) {
        cout << "\n--- Editando a " << member->m_name << " " << member->m_last_name
             << " (ID: " << member->m_id << ") ---\n";
        cout << "1. Nombre: " << member->m_name << "\n";
        cout << "2. Apellido: " << member->m_last_name << "\n";
        cout << "3. Genero: " << member->m_gender << "\n";
        cout << "4. Edad: " << member->m_age << "\n";
        cout << "5. Esta muerto: " << (member->m_is_dead ? "Si" : "No") << "\n";
        cout << "6. En prision: " << (member->m_in_jail ? "Si" : "No") << "\n";
        cout << "7. Fue jefe: " << (member->m_was_boss ? "Si" : "No") << "\n";
        cout << "8. Es jefe: " << (member->m_is_boss ? "Si" : "No") << "\n";
        cout << "0. Salir\n";
        cout << "Seleccione el campo a editar: ";

        int option;
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1: {
                cout << "Ingrese nuevo nombre: ";
                getline(cin, member->m_name);
                break;
            }
            case 2: {
                cout << "Ingrese nuevo apellido: ";
                getline(cin, member->m_last_name);
                break;
            }
            case 3: {
                char g;
                while (true) {
                    cout << "Ingrese nuevo genero (H/M): ";
                    cin >> g;
                    cin.ignore();
                    if (g == 'H' || g == 'M') {
                        member->m_gender = g;
                        break;
                    }
                    cout << "Error: solo H o M\n";
                }
                break;
            }
            case 4: {
                int a;
                while (true) {
                    cout << "Ingrese nueva edad: ";
                    cin >> a;
                    cin.ignore();
                    if (a > 0 && a < 150) {
                        member->m_age = a;
                        break;
                    }
                    cout << "Error: edad fuera de rango.\n";
                }
                break;
            }
            case 5: {
                member->m_is_dead = !member->m_is_dead;
                cout << "Estado cambiado a: " << (member->m_is_dead ? "Muerto" : "Vivo") << "\n";
                break;
            }
            case 6: {
                member->m_in_jail = !member->m_in_jail;
                cout << "Estado cambiado a: " << (member->m_in_jail ? "En prision" : "Libre")
                     << "\n";
                break;
            }
            case 7: {
                member->m_was_boss = !member->m_was_boss;
                cout << "Estado cambiado a: " << (member->m_was_boss ? "Fue jefe" : "No fue jefe")
                     << "\n";
                break;
            }
            case 8: {
                member->m_is_boss = !member->m_is_boss;
                cout << "Estado cambiado a: " << (member->m_is_boss ? "Es jefe" : "Ya no es jefe")
                     << "\n";
                break;
            }
            case 0:
                editing = false;
                break;
            default:
                cout << "Opcion invalida\n";
        }
    }
}
