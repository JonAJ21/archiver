#pragma once

#include <string>

class BWT final {
public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
};