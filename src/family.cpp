#include "family.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

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