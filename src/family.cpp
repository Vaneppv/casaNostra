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
        throw std::runtime_error("La cola esta vacia");
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