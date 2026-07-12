#ifndef FAMILY_HPP
#define FAMILY_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Member {
public:
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_boss;
    bool is_dead, in_jail, was_boss, is_boss;
    
    Member* boss = nullptr;
    Member* left = nullptr;
    Member* right = nullptr;

    Member(int id, string n, string ln, char g, int a, int id_b, bool d, bool j, bool wb, bool ib) {
        this->id = id;
        this->name = n;
        this->last_name = ln;
        this->gender = g;
        this->age = a;
        this->id_boss = id_b;
        this->is_dead = d;
        this->in_jail = j;
        this->was_boss = wb;
        this->is_boss = ib;
    }
};

class FamilyTree {
private:
    Member* root;
    
public:
    FamilyTree() : root(nullptr) {}

    Member* getRoot() { return root; }
};

#endif