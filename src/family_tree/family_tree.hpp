#pragma once

#include <string>

#include "../orphan_queue/orphan_queue.hpp"

class FamilyTree {
private:
    Member* m_root = nullptr;
    OrphanQueue m_orphan_queue;

    void delete_tree(Member* node);
    Member* find_member_by_id_rec(Member* node, int id) const;
    Member* find_first_alive_free(Member* node) const;
    Member* find_first_alive_jailed(Member* node) const;
    Member* find_current_boss(Member* node) const;
    Member* find_successor(Member* boss, bool search_free) const;
    void attach_orphans();

public:
    FamilyTree() = default;
    ~FamilyTree() { delete_tree(m_root); }
    Member* get_root() const { return m_root; }
    void load_from_csv(const std::string& filename);
    Member* find_member_by_id(int id) const;
    void edit_member(int id);
};
