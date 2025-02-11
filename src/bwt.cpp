#include "bwt.h"
#include "suffix_tree.h"

const unsigned char BWT::TERMINATION_SYMBOL = 0;

std::vector<unsigned char> BWT::encode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> result;

    std::vector<unsigned char> text = input;
    text.push_back(TERMINATION_SYMBOL);
    SuffixTree suffixTree(text);
   
    std::vector<Node*> nodes = suffixTree.getNodesDFS();

    for (auto node : nodes) {
        if (node->start == -1) {
            continue;
        }
        unsigned char lastChar = text[*(node->end)];
        
        if (lastChar == TERMINATION_SYMBOL) {
            if (text.size() - node->depth - 1 == -1) {
                result.push_back(TERMINATION_SYMBOL);
                continue;
            }
            result.push_back(text[text.size() - node->depth - 1]);
        }
    }

    return result;
}

std::vector<unsigned char> BWT::decode(const std::vector<unsigned char>& input) {
    int n = input.size();

    std::vector<int> count(256, 0);
    std::vector<int> next(n);
    std::vector<unsigned char> sorted_bwt(n);

    for (unsigned char ch : input) {
        count[ch]++;
    }

    std::vector<int> first(256, 0);
    for (int i = 1; i < 256; ++i) {
        first[i] = first[i - 1] + count[i - 1];
    }

    for (int i = 0; i < n; ++i) {
        sorted_bwt[first[input[i]]] = input[i];
        next[first[input[i]]++] = i;
    }

    std::vector<unsigned char> result;
    int index = next[0];
    for (int i = 0; i < n - 1; ++i) {
        result.push_back(sorted_bwt[index]);
        index = next[index];
    }
    return result;

    // std::wstring original;
    // int index = next[0]; 
    // for (int i = 0; i < n; i++) {
    //     original += sorted_bwt[index];
    //     index = next[index];
    // }

    //return original.substr(0, original.size() - 1);
    // return v;
}