#include "mtf.h"

#include <bitset>

std::vector<unsigned char> MTF::getAlphabet() {
    std::vector<unsigned char> alphabet(256);
    for (unsigned char i = 0; i < 255; ++i) {
        alphabet[i] = i;
    }
    alphabet[255] = 255;
    
    return alphabet;
}

std::vector<unsigned char> MTF::encode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> alphabet = getAlphabet();
    
    std::vector<unsigned char> result;

    for (size_t i = 0; i < input.size(); ++i) {
        auto it = std::find(alphabet.begin(), alphabet.end(), input[i]);
        if (it != alphabet.end()) {
            result.push_back(std::distance(alphabet.begin(), it));
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), input[i]);
        }
    }
    
    return result;
}

std::vector<unsigned char> MTF::decode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> alphabet = getAlphabet();
    
    std::vector<unsigned char> result(input.size());
    size_t i = 0;
    for (unsigned char index : input) {
        unsigned char ch = alphabet[index];
        result[i++] = ch;
        alphabet.erase(alphabet.begin() + index);
        alphabet.insert(alphabet.begin(), ch);
    }
    
    return result;
}