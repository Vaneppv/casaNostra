#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Member {
public:
    int m_id;
    std::string m_name;
    std::string m_last_name;
    char m_gender;
    int m_age;
    int m_id_boss;
    bool m_is_dead;
    bool m_in_jail;
    bool m_was_boss;
    bool m_is_boss;

    Member* m_boss = nullptr;
    Member* m_left = nullptr;
    Member* m_right = nullptr;

    Member(int id, std::string n, std::string ln, char g, int a, int id_b, bool d, bool j, bool wb,
           bool ib)
        : m_id(id), m_name(std::move(n)), m_last_name(std::move(ln)), m_gender(g), m_age(a),
          m_id_boss(id_b), m_is_dead(d), m_in_jail(j), m_was_boss(wb), m_is_boss(ib),
          m_boss(nullptr), m_left(nullptr), m_right(nullptr) {}
};

class FamilyTree {
private:
    Member* m_root;

    void delete_tree(Member* node); 
    Member* find_member_by_id_rec(Member* node, int id) const;  

public:
    FamilyTree() : m_root(nullptr) {}
    ~FamilyTree() { delete_tree(m_root); }
    Member* get_root() const { return m_root; }
    Member* find_member_by_id(int id) const;  // wrapper público
};
