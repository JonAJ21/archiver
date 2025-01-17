#pragma once

#include <string>
#include <iostream>
#include <sstream>

class RLE final {
public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
};