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

    Member(int id, std::string name, std::string last, char gender,
           int age, int id_b, bool dead, bool jail, bool was_b, bool is_b)
        : m_id(id), m_name(std::move(name)), m_last_name(std::move(last)),
          m_gender(gender), m_age(age), m_id_boss(id_b), m_is_dead(dead),
          m_in_jail(jail), m_was_boss(was_b), m_is_boss(is_b) {}
};

class OrphanNode {
public:
    Member* m_member;
    OrphanNode* m_next;

    OrphanNode(Member* member) : m_member(member), m_next(nullptr) {}
};

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
    Member* peek_first () { return this->m_first->m_member; }
    bool empty() const { return m_first == nullptr; }
    int size() const { return m_size; }
};

class FamilyTree {
private:
    Member* m_root = nullptr;
    OrphanQueue m_orphan_queue;

    void delete_tree(Member* node); 
    Member* find_member_by_id_rec(Member* node, int id) const;
    Member* find_first_alive_free(Member* node) const;
    void attach_orphans();  

public:
    FamilyTree() = default;
    ~FamilyTree() { delete_tree(m_root); }
    Member* get_root() const { return m_root; }
    void load_from_csv(const std::string& filename);
    Member* find_member_by_id(int id) const;
    void edit_member(int id);
};