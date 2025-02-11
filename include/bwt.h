#pragma once

#include <vector>

class BWT final {
private: 
    static const unsigned char TERMINATION_SYMBOL;
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
    
};