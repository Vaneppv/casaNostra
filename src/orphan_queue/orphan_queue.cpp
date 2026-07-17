#include "orphan_queue.hpp"

#include <stdexcept>

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
