#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

class MTF final {
private:
    static const size_t ALPHABET_LENGTH = 2048;
    static std::vector<wchar_t> getAlphabet();
public:
    static std::wstring encode(const std::wstring& input);
    static std::wstring decode(const std::wstring& input);
};
