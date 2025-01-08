#include "bwt.h"
#include "suffix_tree.h"
#include "sort.h"

std::string BWT::encode(const std::string& input) {
    std::string result = "";

    std::string text = input + '$';
    SuffixTree suffixTree(text);
   
    std::vector<Node*> nodes = suffixTree.getNodesDFS();
    
    for (auto node : nodes) {
        if (node->start == -1) {
            continue;
        }
        char lastChar = text[*(node->end)];
        if (lastChar == '$') {
            if (text.size() - node->depth - 1 == -1) {
                result += '$';
                continue;
            }
            result += text[text.size() - node->depth - 1];
        }
    }

    return result;
}

std::string BWT::decode(const std::string& input) {
    int n = input.size();
    std::vector<int> count(256, 0);
    std::vector<int> next(n);
    std::vector<char> sorted_bwt(n);

    for (char c : input) {
        count[c]++;
    }

    std::vector<int> first(256, 0);
    for (int i = 1; i < 256; i++) {
        first[i] = first[i - 1] + count[i - 1];
    }

    for (int i = 0; i < n; i++) {
        sorted_bwt[first[input[i]]] = input[i];
        next[first[input[i]]++] = i;
    }

    std::string original;
    int index = next[0]; 
    for (int i = 0; i < n; i++) {
        original += sorted_bwt[index];
        index = next[index];
    }

    return original.substr(0, original.size() - 1);
}