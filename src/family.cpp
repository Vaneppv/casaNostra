#include "family.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Member* FamilyTree::find_member_by_id(Member* node, int id) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->m_id == id) {
        return node;
    }
    Member* found = find_member_by_id(node->m_left, id);
    if (found != nullptr) {
        return found;
    }
    return find_member_by_id(node->m_right, id);
}