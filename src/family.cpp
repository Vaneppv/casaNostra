#include "family.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

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

void OrphanQueue::push(Member* member) {
    OrphanNode* orphan = new OrphanNode(member);
        
    if (m_first == nullptr) {
        m_first = orphan;
        m_last = orphan;
    } else {
        m_last->m_next = orphan; 
        m_last = orphan;         
    }
    m_size++;
}

Member* OrphanQueue::pop() {
    if (m_first == nullptr) {
        throw runtime_error("La cola esta vacia");
    }

    OrphanNode* temp = m_first;
    Member* member = temp->m_member;

    m_first = m_first->m_next;

    if (m_first == nullptr) {
        m_last = nullptr;
    }

    m_size--;
    delete temp;
    return member;
}

void FamilyTree::attach_orphans() {
    bool progress;
    do {
        progress = false;
        OrphanQueue temp;  // cola para los que no se puedan enlazar aún
        while (!m_orphan_queue.empty()) {
            Member* m = m_orphan_queue.pop();
            Member* boss = find_member_by_id(m->m_id_boss);
            if (boss != nullptr) {
                m->m_boss = boss;
                if (boss->m_left == nullptr) boss->m_left = m;
                else if (boss->m_right == nullptr) boss->m_right = m;
                else {
                    cerr << "Jefe " << boss->m_id << " lleno al intentar agregar a " << m->m_id << "\n";
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
        getline(ss, token, ','); id = stoi(token);
        // name
        getline(ss, token, ','); name = token;
        // last_name
        getline(ss, token, ','); last_name = token;
        // gender
        getline(ss, token, ','); gender = token[0];
        
        getline(ss, token, ','); age = stoi(token);
        // id_boss
        getline(ss, token, ','); id_boss = stoi(token);
        // is_dead
        getline(ss, token, ','); is_dead = (stoi(token) == 1);
        // in_jail
        getline(ss, token, ','); in_jail = (stoi(token) == 1);
        // was_boss
        getline(ss, token, ','); was_boss = (stoi(token) == 1);
        // is_boss
        getline(ss, token, ','); is_boss = (stoi(token) == 1);

        Member* new_member = new Member(id, name, last_name, gender, age,
            id_boss, is_dead, in_jail, was_boss, is_boss);

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
                if (boss->m_left == nullptr) boss->m_left = new_member;
                else if (boss->m_right == nullptr) boss->m_right = new_member;
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
        cerr << "Huérfano sin jefe: " << m->m_id << " (jefe " << m->m_id_boss << " no encontrado)\n";
        delete m; // Se liberan al no poder almacenarse
    }
}