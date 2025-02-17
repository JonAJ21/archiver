#include "rle.h"

#include <bitset>

std::vector<unsigned char> RLE::encode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> result;

    size_t i = 0;
    while (i < input.size()) {
        unsigned char ch = input[i];
        unsigned char count = 1;

        while (i + count < input.size() && input[i + count] == ch && count < 255) {
            ++count;
        }

        result.push_back(count);
        result.push_back(ch);
        
        i += count;
    }

    for (auto ch : result) {
        std::cout << std::bitset<8>(ch) << ' ';
    }
    std::cout << std::endl;


    return result;
}

std::vector<unsigned char> RLE::decode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> result;


    for (auto ch : input) {
        std::cout << (int) ch << std::endl;
    }

    if (input.size() % 2 != 0) {
        throw std::invalid_argument("Invalid RLE encoded data");
    }

    for (size_t i = 0; i < input.size(); i += 2) {
        unsigned char count = input[i];
        unsigned char ch = input[i + 1];

        for (unsigned char j = 0; j < count; j++) {
            result.push_back(ch);
        }
    }
    
    return result;
}