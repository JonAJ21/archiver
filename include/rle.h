#pragma once

#include <string>
#include <iostream>
#include <sstream>

class RLE final {
public:
    static std::wstring encode(const std::wstring& input);
    static std::wstring decode(const std::wstring& input);
};