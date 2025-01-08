#include "rle.h"

std::string RLE::encode(const std::string& input) {
    std::string result;
    int count = 1;

    std::istringstream iss(input);
    
    int i = 0;
    int index;
    int prevIndex;
    iss >> prevIndex;
    while (iss >> index) {
        ++i;
        if (index == prevIndex) {
            ++count;
        } else {
            result += std::to_string(count) + ' ' + std::to_string(prevIndex);
            count = 1;
            
            result += '\n';
            
        }
        if (iss.eof()) {
            result += std::to_string(count) + ' ' + std::to_string(index);
        }
        prevIndex = index;
    }

    return result;
}

std::string RLE::decode(const std::string& input) {
    std::istringstream iss(input);

    std::string result;
    int count;
    int index;
    while (iss >> count >> index) {
        while (count > 0) {
            result += std::to_string(index);
            if (!iss.eof() || count > 1) {
                result += '\n';
            }
            --count;
        }
    }
    
    return result;
}