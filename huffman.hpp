#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

class SymbolTable{
    public:
    unordered_map<string, long long> symbols;

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
    bool leaf() { return !left && !right; }
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
    SymbolTable table;
    priority_queue<NodeWrapper> queue;
    int nodeAmount;
    int biggestLen;

    void getQueue();
    void buildTree();

    HuffmanTree(string filename, bool byWord);
    ~HuffmanTree();

    // debug
    // unordered_map<string, string> huffmanCodes;
    // void generateCodes(Node* node, string currentCode);
    // void showTree();

};

