#include "rle.h"

std::wstring RLE::encode(const std::wstring& input) {
    std::wstring result;
    int count = 1;

    std::wistringstream iss(input);
    
    int i = 0;
    int index;
    int prevIndex;
    iss >> prevIndex;
    while (iss >> index) {
        ++i;
        if (index == prevIndex) {
            ++count;
        } else {
            result += std::to_wstring(count) + L' ' + std::to_wstring(prevIndex);
            count = 1;
            
            result += L'\n';
            
        }
        if (iss.eof()) {
            result += std::to_wstring(count) + L' ' + std::to_wstring(index);
        }
        prevIndex = index;
    }

    return result;
}

std::wstring RLE::decode(const std::wstring& input) {
    std::wistringstream iss(input);

    std::wstring result;
    int count;
    int index;
    while (iss >> count >> index) {
        while (count > 0) {
            result += std::to_wstring(index);
            if (!iss.eof() || count > 1) {
                result += L'\n';
            }
            --count;
        }
    }
    
    return result;
}