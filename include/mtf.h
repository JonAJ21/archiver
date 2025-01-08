#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

class MTF final {
private:
    static std::vector<char> getAlphabet();

public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
};
