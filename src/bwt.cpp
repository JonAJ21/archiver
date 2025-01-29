#include "bwt.h"
#include "suffix_tree.h"

std::wstring BWT::encode(const std::wstring& input) {
    std::wstring result = L"";

    std::wstring text = input + TERMINATION_SYMBOL;
    SuffixTree suffixTree(text);
   
    std::vector<Node*> nodes = suffixTree.getNodesDFS();

    for (auto node : nodes) {
        if (node->start == -1) {
            continue;
        }
        char lastChar = text[*(node->end)];
        
        if (lastChar == TERMINATION_SYMBOL) {
            if (text.size() - node->depth - 1 == -1) {
                result += TERMINATION_SYMBOL;
                continue;
            }
            result += text[text.size() - node->depth - 1];
        }
    }

    return result;
}

std::wstring BWT::decode(const std::wstring& input) {
    int n = input.size();

    std::vector<int> count(ALPHABET_LENGTH, 0);
    std::vector<int> next(n);
    std::vector<wchar_t> sorted_bwt(n);

    for (wchar_t c : input) {
        count[c]++;
    }

    std::vector<int> first(ALPHABET_LENGTH, 0);
    for (int i = 1; i < ALPHABET_LENGTH; i++) {
        first[i] = first[i - 1] + count[i - 1];
    }

    for (int i = 0; i < n; i++) {
        sorted_bwt[first[input[i]]] = input[i];
        next[first[input[i]]++] = i;
    }

    std::wstring original;
    int index = next[0]; 
    for (int i = 0; i < n; i++) {
        original += sorted_bwt[index];
        index = next[index];
    }

    return original.substr(0, original.size() - 1);
}