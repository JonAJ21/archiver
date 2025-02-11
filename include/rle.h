#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

class RLE final {
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
};