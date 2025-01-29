#include "mtf.h"

std::vector<wchar_t> MTF::getAlphabet() {
    std::vector<wchar_t> alphabet(ALPHABET_LENGTH);
    for (wchar_t i = 0; i < ALPHABET_LENGTH; ++i) {
        alphabet[i] = i;
    }
    return alphabet;
}

std::wstring MTF::encode(const std::wstring& input) {
    std::vector<wchar_t> alphabet = getAlphabet();
    
    std::wstring result;

    for (int i = 0; i < input.size(); ++i) {
        if (i != 0) {
            result += L'\n';
        }
        auto it = std::find(alphabet.begin(), alphabet.end(), input[i]);
        if (it != alphabet.end()) {
            result += std::to_wstring(std::distance(alphabet.begin(), it));
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), input[i]);
        }
    }

    return result;
}

std::wstring MTF::decode(const std::wstring& input) {
    std::vector<wchar_t> alphabet = getAlphabet();
    
    std::wstring result;

    std::wistringstream iss(input);
    int index;

    while (iss >> index) {
        wchar_t ch = alphabet[index];
        result += ch;
        alphabet.erase(alphabet.begin() + index);
        alphabet.insert(alphabet.begin(), ch);
    }

    return result;
}