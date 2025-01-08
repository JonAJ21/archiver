#include "mtf.h"

std::vector<char> MTF::getAlphabet() {
    std::vector<char> alphabet(27);
    alphabet[0] = '$';
    for (char i = 1; i < alphabet.size(); ++i) {
        alphabet[i] = 'a' + i - 1;
    }
    return alphabet;
}

std::string MTF::encode(const std::string& input) {
    std::vector<char> alphabet = getAlphabet();
    
    std::string result;

    for (int i = 0; i < input.size(); ++i) {
        if (i != 0) {
            result += '\n';
        }
        auto it = std::find(alphabet.begin(), alphabet.end(), input[i]);
        if (it != alphabet.end()) {
            result += std::to_string(std::distance(alphabet.begin(), it));
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), input[i]);
        }
    }

    return result;
}

std::string MTF::decode(const std::string& input) {
    std::vector<char> alphabet = getAlphabet();
    
    std::string result;

    std::istringstream iss(input);
    int index;

    while (iss >> index) {
        char ch = alphabet[index];
        result += ch;
        alphabet.erase(alphabet.begin() + index);
        alphabet.insert(alphabet.begin(), ch);
    }

    return result;
}