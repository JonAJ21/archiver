#pragma once

#include <vector>
#include "suffix_tree.h"

class BWT final {
private: 
    static const STSymbol TERMINATION_SYMBOL;
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
    
};