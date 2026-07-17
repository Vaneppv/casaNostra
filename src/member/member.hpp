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

    Member(int id, std::string name, std::string last, char gender, int age, int id_b, bool dead,
           bool jail, bool was_b, bool is_b)
        : m_id(id), m_name(std::move(name)), m_last_name(std::move(last)), m_gender(gender),
          m_age(age), m_id_boss(id_b), m_is_dead(dead), m_in_jail(jail), m_was_boss(was_b),
          m_is_boss(is_b) {}
};
