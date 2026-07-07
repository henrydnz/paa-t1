#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

// estrutura para armazenar os simolos de um arquivo de texto
class SymbolTable{
    public:

    vector<pair<string, int>> symbols;
    int tokenCount;
    int size;

    SymbolTable(ifstream& file, bool byWord);
    SymbolTable(vector<pair<string, int>> symbols);

    void addSymbol(string str);

    void showTable();
};

// estrutura de no da arvore
struct Node {
    string token;
    int count; 

    Node* left;
    Node* right;

    // construtores
    Node(int count, Node* left, Node* right) : token(""), count(count), left(left), right(right) {}
    Node(string token, int count) : token(token), count(count), left(nullptr), right(nullptr) {}

    bool operator<(const Node& other) const { return count > other.count; }

    bool isLeaf() { return !left && !right; }
};

// wrapper, serve pra salvar frequencia de tokens, que eh inutil na arvore final
// economiza espaco na arvore em si
// struct NodeWrapper {
//     Node* node;
//     int count;

    
//     NodeWrapper(Node* node, long long count) : node(node), count(count) {}

//     // comparador reverso porque a fila ordenada precisa popar os menores
//     bool operator<(const NodeWrapper& other) const { return count > other.count; }
// };

// estrutura da arvore binaria de huffman
class HuffmanTree{    
    public:

    Node* root = nullptr;

    SymbolTable symbolTable;
    unordered_map<string, string> huffmanCodes;

    HuffmanTree(ifstream& file, bool byWord);
    HuffmanTree(vector<pair<string, int>> symbolTable);
    ~HuffmanTree();

    void getQueue();
    void buildTree();

    void generateCodes(Node* node, string currentCode);

    void showTree();

};

