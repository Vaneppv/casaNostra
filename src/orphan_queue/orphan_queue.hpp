#include "../orphan_node/orphan_node.hpp"

class OrphanQueue {
private:
    OrphanNode* m_first;
    OrphanNode* m_last;
    int m_size;

public:
    OrphanQueue() : m_first(nullptr), m_last(nullptr), m_size(0) {}

    ~OrphanQueue() {
        while (m_first != nullptr) {
            pop();
        }
    }

    void push(Member* member);
    Member* pop();
    Member* peek_first() { return this->m_first->m_member; }
    bool empty() const { return m_first == nullptr; }
    int size() const { return m_size; }
};
