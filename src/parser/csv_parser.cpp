#include "csv_parser.hpp"

#include <sstream>
#include <string>

Member* CSVParser::parse_csv_line(const std::string& line) {
    std::stringstream str_stream(line);
    std::string token;

    getline(str_stream, token, ',');
    int id = stoi(token);

    getline(str_stream, token, ',');
    std::string name = token;

    getline(str_stream, token, ',');
    std::string last_name = token;

    getline(str_stream, token, ',');
    char gender = token[0];

    getline(str_stream, token, ',');
    int age = stoi(token);

    getline(str_stream, token, ',');
    int id_boss = stoi(token);

    getline(str_stream, token, ',');
    bool is_dead = (stoi(token) == 1);

    getline(str_stream, token, ',');
    bool in_jail = (stoi(token) == 1);

    getline(str_stream, token, ',');
    bool was_boss = (stoi(token) == 1);

    getline(str_stream, token, ',');
    bool is_boss = (stoi(token) == 1);

    return new Member(id, name, last_name, gender, age, id_boss, is_dead, in_jail, was_boss,
                      is_boss);
}
