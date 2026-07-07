#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

class SymbolTable{
    public:
    vector<pair<string, int>> symbols;
    int tokenCount;
    int size;

    SymbolTable(string filename, bool byWord);
    void addSymbol(string str);
    void showTable();
};

struct Node {
    string token;
    Node* left;
    Node* right;

    Node(string token, Node* left, Node* right) : 
        token(token), left(left), right(right) {}
    bool isLeaf() { return !left && !right; }
};

// serve pra salvar frequencia de tokens, que eh inutil na arvore final e ocuparia mt espaco
struct NodeWrapper {
    Node* node;
    long long count;

    NodeWrapper(Node* node, long long count) : node(node), count(count) {}
    // comparador reverso porque a fila ordenada precisa popar os menores
    bool operator<(const NodeWrapper& other) const { return count > other.count; }
};

class HuffmanTree{    
    public:

    Node* root = nullptr;
    SymbolTable symbolTable;
    priority_queue<NodeWrapper> queue;
    unordered_map<string, string> huffmanCodes;

    void getQueue();
    void buildTree();

    HuffmanTree(string filename, bool byWord);
    ~HuffmanTree();

    int getSymbolAmount();
    void generateCodes(Node* node, string currentCode);
    //void showTree();

};

