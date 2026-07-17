#include "../member/member.hpp"

class OrphanNode {
public:
    Member* m_member;
    OrphanNode* m_next {nullptr};

    OrphanNode(Member* member) : m_member(member), m_next(nullptr) {}
};
