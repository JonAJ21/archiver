#include "bwt.h"

const STSymbol BWT::TERMINATION_SYMBOL = {0, 0};

std::vector<unsigned char> BWT::encode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> result;

    std::vector<STSymbol> text(input.size());
    size_t i = 0;
    for (auto ch : input) {
        text[i++] = {ch, 1};
    }
    text.push_back(TERMINATION_SYMBOL);
    SuffixTree suffixTree(text);
   
    std::vector<Node*> nodes = suffixTree.getNodesDFS();
    uint terminationSymbolIndex;
    for (auto node : nodes) {
        if (node->start == -1) {
            continue;
        }
        STSymbol lastChar = text[*(node->end)];
        
        if (lastChar == TERMINATION_SYMBOL) {
            if (text.size() - node->depth - 1 == -1) {
                result.push_back(TERMINATION_SYMBOL.symbol);
                terminationSymbolIndex = result.size() - 1;
                // std::cout << "Term:" <<terminationSymbolIndex << std::endl;
                continue;
            }
            result.push_back(text[text.size() - node->depth - 1].symbol);
        }
    }
    unsigned char bytes[4] = {static_cast<unsigned char>((terminationSymbolIndex >> 24) & 0xFF),
                              static_cast<unsigned char>((terminationSymbolIndex >> 16) & 0xFF),
                              static_cast<unsigned char>((terminationSymbolIndex >> 8) & 0xFF),
                              static_cast<unsigned char>((terminationSymbolIndex) & 0xFF)};
    for (auto ch : bytes) {
        result.push_back(ch);
    }
    return result;
}

std::vector<unsigned char> BWT::decode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> preparedInput(input.size() - 4);
    // size_t i = 0;
    uint terminationSymbolIndex = (static_cast<uint>(input[input.size() - 4]) << 24) |
                                  (static_cast<uint>(input[input.size() - 3]) << 16) |
                                  (static_cast<uint>(input[input.size() - 2]) << 8)  |
                                  (static_cast<uint>(input[input.size() - 1]));
    for (size_t i = 0; i < preparedInput.size(); ++i) {
        preparedInput[i] = input[i];
        // std::cout << preparedInput[i] << ' ';
    }
    
    // std::cout << std::endl;
    
    int n = preparedInput.size();
    
    std::vector<int> count(257, 0);

    for (int i = 0; i < n; ++i) {
        if (i == terminationSymbolIndex) {
            count[0]++;
            continue;
        }
        count[preparedInput[i] + 1]++;
    }

    // for (int i = 0; i < 257; ++i) {
    //     std::cout << count[i] << ' ';
    // }

    int sum = 0;
    for (int i = 0; i < 257; ++i) {
        sum = sum + count[i];
        count[i] = sum - count[i];
    }
    
    std::vector<int> t(n);
    for (int i = 0; i < n; ++i) {
        if (i == terminationSymbolIndex) {
            t[count[0]] = i;
            count[0]++;
            continue;
        }
        t[count[preparedInput[i] + 1]] = i;
        count[preparedInput[i] + 1]++;
    }

    int j = t[terminationSymbolIndex];
    std::vector<unsigned char> result(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        result[i] = preparedInput[j];
        j = t[j];
    }

    return result;
}