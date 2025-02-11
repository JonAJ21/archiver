#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

class MTF final {
private:
    static std::vector<unsigned char> getAlphabet();
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
};
