#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

// estrutura de no da arvore
class Node {
    public:

    string token;
    int count; 

    Node* left;
    Node* right;

    Node(int count, Node* left, Node* right) : token(""), count(count), left(left), right(right) {}
    Node(string token, int count) : token(token), count(count), left(nullptr), right(nullptr) {}

    bool operator<(const Node& other) const { return count > other.count; }

    bool isLeaf() { return !left && !right; }
};

// estrutura da arvore binaria de huffman
class HuffmanTree{    
    private:

    void addSymbol(string symbol);
    void buildSymbolTable(ifstream& file, bool byWord);
    void buildTree();

    public:

    Node* root = nullptr;

    //SymbolTable symbolTable;
    vector<pair<string, int>> symbols;
    int tokenCount;
    int leafCount;

    unordered_map<string, string> huffmanCodes;

    HuffmanTree(ifstream& file, bool byWord);
    HuffmanTree(vector<pair<string, int>> symbolTable);
    ~HuffmanTree();

    void generateCodes(Node* node, string currentCode);

    void showSymbols();
    void showCodes();
    void showTree();
};

