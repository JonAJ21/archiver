#include <bits/stdc++.h>

struct STSymbol {
    unsigned char symbol;
    unsigned char is_termination; // 0 -> true , 1 -> false

    bool operator==(const STSymbol& other) const {
        return symbol == other.symbol && is_termination == other.is_termination;
    }

    bool operator<(const STSymbol& other) const {
        return symbol < other.symbol || (symbol == other.symbol && is_termination < other.is_termination);
    }
};

struct Node {
    Node(Node *link, int start, int *end);
    Node(Node *link, int start, int *end, int ind);
    
    std::map<STSymbol, Node*> children;
    Node *suffixLink;
    int start;
    int *end;
    int suffixIndex;
    int depth;
};


class SuffixTree { 
public: 
    SuffixTree(const std::vector<STSymbol>& input);
    ~SuffixTree();
    std::vector<Node*> getNodesDFS();

private:    
    Node *root = new Node(nullptr, -1, new int(-1));
    Node *lastCreatedInternalNode = nullptr;
    Node *activeNode = nullptr;
    
    std::vector<STSymbol> text;

    int activeEdge = -1;
    int activeLength = 0;
    int remainingSuffixCount = 0;
    int leafEnd = -1;

    void dfs(std::vector<Node*>& nodes);
    void buildSuffixTree();
    void extendSuffixTree(int phase);
    void deleteSuffixTree(Node *node);
    void countIndex(Node *node, std::vector<int>& vec);
    int getEdgeLength(Node *node);
};


Node::Node(Node *link, int start, int *end) 
    : suffixLink(link),
      start(start), 
      end(end), 
      suffixIndex(-1) {}


Node::Node(Node *link, int start, int *end, int index)
    : suffixLink(link),
      start(start), 
      end(end), 
      suffixIndex(index) {}


SuffixTree::SuffixTree(const std::vector<STSymbol>& input) : text(input) {
    buildSuffixTree();
}

SuffixTree::~SuffixTree() {
    deleteSuffixTree(root);
}

std::vector<Node*> SuffixTree::getNodesDFS() {
    std::vector<Node*> result;
    result.push_back(root);
    root->depth = 0;
    dfs(result);
    return result;
}

void SuffixTree::dfs(std::vector<Node*>& result) {
    Node* node = result.back();
    for (auto child : node->children) {
        child.second->depth = node->depth + getEdgeLength(child.second);
        result.push_back(child.second);
        dfs(result);
    }
}

void SuffixTree::buildSuffixTree() {
    activeNode = root;
    for (size_t i = 0; i < text.size(); ++i) {
        extendSuffixTree(i);
    }
}

void SuffixTree::extendSuffixTree(int phase) { 
    lastCreatedInternalNode = nullptr;
    ++leafEnd;
    ++remainingSuffixCount;

    while (remainingSuffixCount > 0) {
        if (activeLength == 0) {
            activeEdge = phase;
        }
        
        auto find = activeNode->children.find(text[activeEdge]);
        
        if (find == activeNode->children.end()) {
            activeNode->children.insert(std::make_pair(
                text[activeEdge],
                new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)
            ));
            if (lastCreatedInternalNode != nullptr) {
                lastCreatedInternalNode->suffixLink = activeNode;
                lastCreatedInternalNode = nullptr;
            }
        } else {
            Node *next = find->second;
            int edgeLength = getEdgeLength(next);

            if (activeLength >= edgeLength) {
                activeEdge += edgeLength;
                activeLength -= edgeLength;
                activeNode = next;
                continue;
            }

            if (text[next->start + activeLength] == text[phase]) {
                if (lastCreatedInternalNode != nullptr && activeNode != root) {
                    lastCreatedInternalNode->suffixLink = activeNode;
                }
                ++activeLength;
                break;
            }

            Node *split = new Node(root, next->start, new int(next->start + activeLength - 1));
            activeNode->children[text[activeEdge]] = split;
            next->start += activeLength;
            split->children.insert(std::make_pair(
                text[phase],
                new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)
            ));
            split->children.insert(std::make_pair(
                text[next->start], 
                next
            ));
            if (lastCreatedInternalNode != nullptr) {
                lastCreatedInternalNode->suffixLink = split;
            }
            lastCreatedInternalNode = split;
        }
        --remainingSuffixCount;
        if (activeNode == root && activeLength > 0) {
            --activeLength;
            ++activeEdge;
        } else if (activeNode != root) {
            activeNode = activeNode->suffixLink;
        }
    }
}

void SuffixTree::deleteSuffixTree(Node *node) {
    for (auto it : node->children) {
        deleteSuffixTree(it.second);
    }
    if (node->suffixIndex == -1)  {
        delete node->end;
    }
    delete node;
}

void SuffixTree::countIndex(Node *node, std::vector<int>& vec) {
    if (!node) {
        return;
    }
    for (auto it : node->children) {
        countIndex(it.second, vec);
    }
    if (node->suffixIndex != -1) {
        vec.push_back(node->suffixIndex);
    }
}

int SuffixTree::getEdgeLength(Node *node) {
    return *(node->end) - node->start + 1;
}


class BWT final {
private: 
    static const STSymbol TERMINATION_SYMBOL;
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
    
};

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

class MTF final {
private:
    static std::vector<unsigned char> getAlphabet();
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
};

std::vector<unsigned char> MTF::getAlphabet() {
    std::vector<unsigned char> alphabet(256);
    for (unsigned char i = 0; i < 255; ++i) {
        alphabet[i] = i;
    }
    alphabet[255] = 255;
    
    return alphabet;
}

std::vector<unsigned char> MTF::encode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> alphabet = getAlphabet();
    
    std::vector<unsigned char> result;

    for (size_t i = 0; i < input.size(); ++i) {
        auto it = std::find(alphabet.begin(), alphabet.end(), input[i]);
        if (it != alphabet.end()) {
            result.push_back(std::distance(alphabet.begin(), it));
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), input[i]);
        }
    }
    
    return result;
}

std::vector<unsigned char> MTF::decode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> alphabet = getAlphabet();
    
    std::vector<unsigned char> result(input.size());
    size_t i = 0;
    for (unsigned char index : input) {
        unsigned char ch = alphabet[index];
        result[i++] = ch;
        alphabet.erase(alphabet.begin() + index);
        alphabet.insert(alphabet.begin(), ch);
    }
    
    return result;
}


class RLE final {
public:
    static std::vector<unsigned char> encode(const std::vector<unsigned char>& input);
    static std::vector<unsigned char> decode(const std::vector<unsigned char>& input);
};

std::vector<unsigned char> RLE::encode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> result;

    size_t i = 0;
    while (i < input.size()) {
        unsigned char ch = input[i];
        unsigned char count = 1;

        while (i + count < input.size() && input[i + count] == ch && count < 255) {
            ++count;
        }

        result.push_back(count);
        result.push_back(ch);
        
        i += count;
    }
    return result;
}

std::vector<unsigned char> RLE::decode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> result;

    if (input.size() % 2 != 0) {
        throw std::invalid_argument("Invalid RLE encoded data");
    }

    for (size_t i = 0; i < input.size(); i += 2) {
        unsigned char count = input[i];
        unsigned char ch = input[i + 1];

        for (unsigned char j = 0; j < count; j++) {
            result.push_back(ch);
        }
    }
    
    return result;
}


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

    std::unordered_map<unsigned char, std::string> huffmanCodes;
    
    buildHuffmanCodes(huffmanTree, "", huffmanCodes);

    if (huffmanCodes.size() == 1) {
        huffmanCodes.begin()->second = "1";
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

    huffmanTree->deleteTree();
    delete bitIndex;
    return result;
}

std::vector<unsigned char> Huffman::decode(const std::vector<unsigned char>& input) {
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

    std::vector<unsigned char> result;

    unsigned char bitsInLastByte = input.back();
    std::string code = "";
    for (size_t i = *currentByteIndex; i < input.size() - 1; ++i) {
        unsigned char j = 0;
        unsigned char jMax = 8;
        if (i == *currentByteIndex) {
            j = *bitIndex;
        }
        if (i == input.size() - 2) {
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
                code = "";
            }
        }

    }

    if (huffmanTree) {
        huffmanTree->deleteTree();
    }
    delete currentByteIndex;
    delete bitIndex;

    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "There must be 3 arguments:\n 1) compress/decompress\n 2) path to input file\n 3) path to output file\n";
    }

    std::string command = argv[1];
    std::string inputFilePath = argv[2];
    std::string outputFilePath = argv[3];
    
    bool compress;
    if (command == "compress") {
        compress = true;
    } else if (command == "decompress") {
        compress = false;
    } else {
        std::cout << "Error: invalid command" << std::endl;
        return 1;
    }

    std::ifstream in(inputFilePath, std::ios_base::in | std::ios_base::binary);

    if (!in.is_open()) {
        std::cout << "Error: input file could not open" << std::endl;
        return 1;
    }

    in.seekg(0, std::ios::end);
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);


    
    std::ofstream out(outputFilePath, std::ios_base::out);

    if (!out.is_open()) {
        std::cout << "Error: output file could not open" << std::endl;
        return 2;
    }

    if (size == 0) {
        return 0;
    }


    if (compress == true) {
        size_t readedBytes = 0;
        while (readedBytes < size) {
            uint bytesToRead = std::min(size - readedBytes, (size_t) 1024 * 1024 * 8);
            std::vector<unsigned char> buffer(bytesToRead);

            if (!in.read(reinterpret_cast<char*>(buffer.data()), bytesToRead)) {
                std::cout << "Error: read" << std::endl;
                return 3;
            }
            readedBytes += bytesToRead;
            std::vector<unsigned char> encoded = BWT::encode(buffer);
            encoded = MTF::encode(encoded);
            encoded = RLE::encode(encoded);
            encoded = Huffman::encode(encoded);
            
            unsigned char bytes[4] = {static_cast<unsigned char>((encoded.size() >> 24) & 0xFF),
                                      static_cast<unsigned char>((encoded.size() >> 16) & 0xFF),
                                      static_cast<unsigned char>((encoded.size() >> 8) & 0xFF),
                                      static_cast<unsigned char>((encoded.size()) & 0xFF)};
            out << bytes[0];
            out << bytes[1];
            out << bytes[2];
            out << bytes[3];

            for (auto ch : encoded) {
                out << ch;
            }

            std::cout << "Encoded bytes: " << readedBytes << std::endl;
        }
    } else {
        size_t readedBytes = 0;
        while (readedBytes < size) {
            std::vector<unsigned char> bytes(4);
            if (!in.read(reinterpret_cast<char*>(bytes.data()), 4)) {
                std::cout << "Error: read" << std::endl;
                return 3;
            }
            uint bytesToRead = (static_cast<uint>(bytes[0]) << 24) |
                               (static_cast<uint>(bytes[1]) << 16) |
                               (static_cast<uint>(bytes[2]) << 8)  |
                               (static_cast<uint>(bytes[3]));
            
            std::vector<unsigned char> buffer(bytesToRead);

            if (!in.read(reinterpret_cast<char*>(buffer.data()), bytesToRead)) {
                std::cout << "Error: read" << std::endl;
                return 3;
            }
            readedBytes += bytesToRead + 4;
            
            std::vector<unsigned char> decoded = Huffman::decode(buffer);
            decoded = RLE::decode(decoded);
            decoded = MTF::decode(decoded);
            decoded = BWT::decode(decoded);

            for (auto ch : decoded) {
                out << ch;
            }

            std::cout << "Decoded bytes: " << readedBytes << std::endl;
        }
    }



    in.close();
    out.close();


    return 0;
}