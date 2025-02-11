#pragma once

#include <vector>
#include <bitset>

class BWT final {
public:
    static std::vector<std::bitset<8>> encode(const std::vector<std::bitset<8>>& input);
    static std::vector<std::bitset<8>> decode(const std::vector<std::bitset<8>>& input);
};