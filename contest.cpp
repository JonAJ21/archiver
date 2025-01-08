#include <bits/stdc++.h>

struct Node;
class SuffixTree;

struct Node {
    Node(Node *link, int start, int *end);
    Node(Node *link, int start, int *end, int ind);
    
    std::map<char, Node*> children;
    Node *suffixLink;
    int start;
    int *end;
    int suffixIndex;
    int depth;
};

class SuffixTree { 
public: 
    SuffixTree(const std::string& input);
    ~SuffixTree();
    std::vector<Node*> getNodesDFS();


private:
    // static const char TERMINATION_SYMBOL = '$';
    
    Node *root = new Node(nullptr, -1, new int(-1));
    Node *lastCreatedInternalNode = nullptr;
    Node *activeNode = nullptr;
    
    std::string text;

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


SuffixTree::SuffixTree(const std::string& input) : text(input) {
    // text += TERMINATION_SYMBOL;
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


class Sort {
public:
    static void radixSort(std::vector<std::string>& arr);
private:
    static char getCharAt(const std::string& str, int index);
};

void Sort::radixSort(std::vector<std::string>& arr) {
    int maxLength = 0;
    for (const auto &str : arr) {
        maxLength = std::max(maxLength, static_cast<int>(str.size()));
    }

    for (int pos = maxLength - 1; pos >= 0; --pos) {
        std::vector<std::vector<std::string>> buckets(256);

        for (const auto &str : arr) {
            char ch = getCharAt(str, pos);
            buckets[ch].push_back(str);
        }
        arr.clear();
        for (const auto &bucket : buckets) {
            for (const auto &str : bucket) {
                arr.push_back(str);
            }
        }
    }
}
char Sort::getCharAt(const std::string& str, int index) {
    if (index < str.size()) {
        return str[index];
    }
    return '\0';
}


class BWT final {
public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
};

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

class MTF final {
private:
    static std::vector<char> getAlphabet();

public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
};


std::vector<char> MTF::getAlphabet() {
    std::vector<char> alphabet(27);
    alphabet[0] = '$';
    for (char i = 1; i < alphabet.size(); ++i) {
        alphabet[i] = 'a' + i - 1;
    }
    return alphabet;
}

std::string MTF::encode(const std::string& input) {
    std::vector<char> alphabet = getAlphabet();
    
    std::string result;

    for (int i = 0; i < input.size(); ++i) {
        if (i != 0) {
            result += '\n';
        }
        auto it = std::find(alphabet.begin(), alphabet.end(), input[i]);
        if (it != alphabet.end()) {
            result += std::to_string(std::distance(alphabet.begin(), it));
            alphabet.erase(it);
            alphabet.insert(alphabet.begin(), input[i]);
        }
    }

    return result;
}

std::string MTF::decode(const std::string& input) {
    std::vector<char> alphabet = getAlphabet();
    
    std::string result;

    std::istringstream iss(input);
    int index;

    while (iss >> index) {
        char ch = alphabet[index];
        result += ch;
        alphabet.erase(alphabet.begin() + index);
        alphabet.insert(alphabet.begin(), ch);
    }

    return result;
}


class RLE final {
public:
    static std::string encode(const std::string& input);
    static std::string decode(const std::string& input);
};

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

int main() {

    std::string command;
    std::cin >> command;
    if (command == "compress") {
        std::string input;
    
        std::string str;
        while (std::cin >> str) {
            input += str;
        }

        if (input.size() == 0) {
            return 0;
        }

        std::string bwtOutput = BWT::encode(input);
        std::string mtfOutput = MTF::encode(bwtOutput);
        std::string rleOutput = RLE::encode(mtfOutput);

        std::cout << rleOutput << std::endl;

    } else if (command == "decompress") {
        std::string input;
        std::string str;
        while (std::cin >> str) {
            input += str + '\n';
        }        

        std::string rleOutput = RLE::decode(input);
        std::string mtfOutput = MTF::decode(rleOutput);
        std::string bwtOutput = BWT::decode(mtfOutput);
        
        std::cout << bwtOutput << std::endl;
    }

    return 0;
}