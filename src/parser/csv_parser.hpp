#pragma once

#include "../member/member.hpp"

class CSVParser {
public:
    CSVParser() = default;
    static Member* parse_csv_line(const std::string& line);
};
