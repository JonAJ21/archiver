#pragma once

#include <string>
//#include <limits>

class BWT final {
private: 
    static const wchar_t TERMINATION_SYMBOL = 0;
    //static const int WCHAR_LIMIT = WCHAR_MAX; 
    static const int ALPHABET_LENGTH = 2048;
public:
    static std::wstring encode(const std::wstring& input);
    static std::wstring decode(const std::wstring& input);
    
};