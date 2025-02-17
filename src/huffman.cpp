#include "huffman.h"

HuffmanNode::HuffmanNode(unsigned char symbol, bool is_artificial, size_t frequency) {
    this->symbol = {symbol, is_artificial};
    this->frequency = frequency;
    left = right = nullptr;
}

void HuffmanNode::deleteTree() {
    if (this->left) {
        this->left->deleteTree();
    }
    if (this->right) {
        this->right->deleteTree();
    }
    delete this;
}

bool HuffmanNodeComparator::operator()(HuffmanNode* left, HuffmanNode* right) {
    return left->frequency > right->frequency;
}

void Huffman::buildHuffmanCodes(HuffmanNode* node, std::string code, std::unordered_map<unsigned char, std::string>& codes) {
    if (!node) return;

    if (!node->symbol.is_artificial) {
        codes[node->symbol.symbol] = code;
    }

    buildHuffmanCodes(node->left, code + "0", codes);
    buildHuffmanCodes(node->right, code + "1", codes);
}

void Huffman::buildHuffmanCodes(HuffmanNode* node, std::string code, std::unordered_map<std::string, unsigned char>& codes) {
    if (!node) return;

    if (!node->symbol.is_artificial) {
        codes[code] = node->symbol.symbol;
    }

    buildHuffmanCodes(node->left, code + "0", codes);
    buildHuffmanCodes(node->right, code + "1", codes);
}


HuffmanNode* Huffman::buildHuffmanTree(const std::vector<unsigned char>& text) {
    std::unordered_map<unsigned char, size_t> frequencies;
    for (unsigned char ch : text) {
        frequencies[ch]++;
    }

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanNodeComparator> pq;

    for (auto pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, false, pair.second));
    }

    while (pq.size() != 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        
        HuffmanNode* newNode = new HuffmanNode(0, true, left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);        
    }

    return pq.top();
}

void Huffman::serializeTree(HuffmanNode* node, std::vector<unsigned char>& result, unsigned char* bitIndex) {
    if (node->left == nullptr && node->right == nullptr) { 
        result.push_back(0);
        if (*bitIndex % 8 == 0) {
            result.push_back(0);
        }
        
        size_t resultSize = result.size();

        std::bitset<8> prevByte = result[resultSize - 2];
        std::bitset<8> currentByte = result[resultSize - 1];
        std::bitset<8> symbolByte = node->symbol.symbol;

        prevByte.set(7 - *bitIndex % 8, 1);
        ++(*bitIndex);

        unsigned char i = 8;
        while (*bitIndex % 8 != 0) {
            prevByte.set(7 - *bitIndex % 8, symbolByte[--i]);
             
            ++(*bitIndex);
        }
        
        while (i > 0) {
            currentByte.set(7 - *bitIndex % 8, symbolByte[--i]);

            ++(*bitIndex);
        }

        result[resultSize - 2] = static_cast<unsigned char>(prevByte.to_ulong());
        result[resultSize - 1] = static_cast<unsigned char>(currentByte.to_ulong());

        *bitIndex = (*bitIndex) % 8;
        
        return;
    }
        
    if (*bitIndex % 8 == 0) {
        result.push_back(0);
    }
    size_t resultSize = result.size();
    std::bitset<8> currentByte = result[resultSize - 1];
    currentByte.set(7 - *bitIndex % 8, 0);

    result[resultSize - 1] = static_cast<unsigned char>(currentByte.to_ulong());

    *bitIndex = (++(*bitIndex)) % 8;

    serializeTree(node->left, result, bitIndex);
    serializeTree(node->right, result, bitIndex);
}

HuffmanNode* Huffman::deserializeTree(const std::vector<unsigned char>& text, size_t* currentByteIndex, unsigned char* bitIndex) {
    std::bitset<8> currentByte = text[*currentByteIndex];
    
    if (currentByte.test(7 - *bitIndex % 8)) {
        ++(*bitIndex);
        if (*bitIndex % 8 == 0) {
            *bitIndex = 0;
            ++(*currentByteIndex);
            currentByte = text[*currentByteIndex];
        }

        std::bitset<8> symbol(0);

        unsigned char i = 8;
        
        while (*bitIndex % 8 != 0) {
            symbol.set(i - 1, currentByte[7 - *bitIndex % 8]);

            ++(*bitIndex);
            if (*bitIndex % 8 == 0) {
                *bitIndex = 0;
                ++(*currentByteIndex);
                currentByte = text[*currentByteIndex];
            }
            --i;
        }

        while (i > 0) {
            symbol.set(i - 1, currentByte[7 - *bitIndex % 8]);

            ++(*bitIndex);
            if (*bitIndex % 8 == 0) {
                *bitIndex = 0;
                ++(*currentByteIndex);
                currentByte = text[*currentByteIndex];
            }
            --i;
        }
        
        *bitIndex = *bitIndex % 8;

        return new HuffmanNode(static_cast<unsigned char>(symbol.to_ulong()), false, 1);    
    }

    ++(*bitIndex);
    if (*bitIndex % 8 == 0) {
        *bitIndex = 0;
        ++(*currentByteIndex);
        currentByte = text[*currentByteIndex];
    }
    
    HuffmanNode* node = new HuffmanNode(0, true, 1);

    node->left = deserializeTree(text, currentByteIndex, bitIndex);
    node->right = deserializeTree(text, currentByteIndex, bitIndex);

    return node;
}

std::vector<unsigned char> Huffman::encode(const std::vector<unsigned char>& input) {
    HuffmanNode* huffmanTree = buildHuffmanTree(input);
    
    
    std::cout << std::endl;

    std::unordered_map<unsigned char, std::string> huffmanCodes;
    
    buildHuffmanCodes(huffmanTree, "", huffmanCodes);

    if (huffmanCodes.size() == 1) {
        huffmanCodes.begin()->second = "1";
    }
    for (auto ch : huffmanCodes) {
        std::cout << std::bitset<8>(ch.first) << ' ' << ch.second << std::endl;
    }

    std::vector<unsigned char> result;
    unsigned char* bitIndex = new unsigned char('\0');
    serializeTree(huffmanTree, result, bitIndex);

    
    for (auto ch : input) {
        std::string chCode = huffmanCodes[ch];
        std::bitset<8> currentByte = result[result.size() - 1];

        for (auto bit : chCode) {
            if (*bitIndex % 8 == 0) {
                result[result.size() - 1] = static_cast<unsigned char>(currentByte.to_ulong());
                result.push_back(0);
                currentByte = result[result.size() - 1];
            }

            currentByte.set(7 - *bitIndex % 8, bit - '0');
            ++(*bitIndex);
        }
        result[result.size() - 1] = static_cast<unsigned char>(currentByte.to_ulong());
        *bitIndex = (*bitIndex) % 8;
    }

    result.push_back(*bitIndex);
    std::cout << "\n";
    for (auto ch : result) {
        std::cout << std::bitset<8>(ch) << ' ';
    }
    std::cout << std::endl;



    huffmanTree->deleteTree();
    delete bitIndex;
    return result;
}

std::vector<unsigned char> Huffman::decode(const std::vector<unsigned char>& input) {
    
    for (auto ch : input) {
        std::cout << std::bitset<8>(ch) << ' ';
    }
    std::cout << std::endl;


    size_t* currentByteIndex = new size_t(0);
    unsigned char* bitIndex = new unsigned char('\0');

    HuffmanNode* huffmanTree = deserializeTree(input, currentByteIndex, bitIndex);
    
    std::unordered_map<std::string, unsigned char> huffmanCodes;
    
    buildHuffmanCodes(huffmanTree, "", huffmanCodes);

    if (huffmanCodes.size() == 1) {
        auto ch = huffmanCodes.begin()->second;
        huffmanCodes.clear();
        huffmanCodes["1"] = ch;
    }
    std::cout << "Codes: " << std::endl;
    for (auto ch : huffmanCodes) {
        std::cout << ch.first << ' ' << (int)ch.second << std::endl;
    }
    std::cout << std::endl;



    std::vector<unsigned char> result;

    unsigned char bitsInLastByte = input.back();
    std::cout << "InputSize:" << input.size() << std::endl;
    std::string code = "";
    for (size_t i = *currentByteIndex; i < input.size() - 1; ++i) {
        // std::cout << i << ' ' << std::bitset<8>(input[i]) << std::endl;
        // std::cout << (int)*bitIndex << std::endl;
        unsigned char j = 0;
        unsigned char jMax = 8;
        if (i == *currentByteIndex) {
            j = *bitIndex;
        }
        if (i == input.size() - 2) {
            std::cout << "Bits in last byte: " << (int)bitsInLastByte << std::endl;
            jMax = bitsInLastByte;
            if (bitsInLastByte == 0) {
                jMax = 8;
            }
        }
        std::bitset<8> currentByte = input[i];
        for (; j < jMax; ++j){
            if (currentByte.test(7 - j)) {
                code += '1';
            } else {
                code += '0';
            }
            
            if (auto search = huffmanCodes.find(code); search != huffmanCodes.end()) {
                result.push_back(search->second);
                std::cout << search->first << ' ' << std::bitset<8>(search->second) << ' ' << (int)j << ' ' << (int)jMax << ' ' << (int) i << ' ' << currentByte<< std::endl;
                code = "";
            }
        }

    }




    for (auto ch : result) {
        std::cout << std::bitset<8>(ch) << ' ';
    }

    std::cout << "\n=================== \n" << result.size() << std::endl;

    if (huffmanTree) {
        huffmanTree->deleteTree();
    }
    delete currentByteIndex;
    delete bitIndex;

    return result;
}