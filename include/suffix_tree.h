#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>


struct Node;
class SuffixTree;

struct Node {
    Node(Node *link, int start, int *end);
    Node(Node *link, int start, int *end, int ind);
    
    std::map<unsigned char, Node*> children;
    Node *suffixLink;
    int start;
    int *end;
    int suffixIndex;
    int depth;
};


class SuffixTree { 
public: 
    SuffixTree(const std::vector<unsigned char>& input);
    ~SuffixTree();
    std::vector<Node*> getNodesDFS();

private:    
    Node *root = new Node(nullptr, -1, new int(-1));
    Node *lastCreatedInternalNode = nullptr;
    Node *activeNode = nullptr;
    
    std::vector<unsigned char> text;

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