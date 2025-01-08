#include "suffix_tree.h"

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
