#pragma once

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <iostream>

struct Symbol {
    unsigned char symbol;
    bool is_artificial;
};

struct HuffmanNode {
    Symbol symbol;
    size_t frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char symbol, bool is_artificial, size_t frequency);
    
    void deleteTree();
};

struct HuffmanNodeComparator {
    bool operator()(HuffmanNode* left, HuffmanNode* right);
};

class Huffman final {
private:
    static void buildHuffmanCodes(HuffmanNode* node, std::string code, std::unordered_map<unsigned char, std::string>& codes);
    static void buildHuffmanCodes(HuffmanNode* node, std::string code, std::unordered_map<std::string, unsigned char>& codes);
    
    static HuffmanNode* buildHuffmanTree(const std::vector<unsigned char>& text);

    static void serializeTree(HuffmanNode* node, std::vector<unsigned char>& result, unsigned char* bitIndex);
    static HuffmanNode* deserializeTree(const std::vector<unsigned char>& text, size_t* currentByteIndex, unsigned char* bitIndex);
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
};
