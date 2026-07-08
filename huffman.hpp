/**
 * @file huffman.hpp
 * @brief Estruturas de dados para a criação e manipulação da Árvore de Huffman.
 * @authors Henrique Diniz da Costa e Noam Daniel Wandscheer Brecher
 */

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// estrutura de no da arvore
class Node {
    public:

    string token;
    int count; 

    Node* left;
    Node* right;

    // criar no de caminho
    Node(int count, Node* left, Node* right) : token(""), count(count), left(left), right(right) {}
    // criar no folha
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

    vector<pair<string, int>> symbols;
    int tokenCount; // numero de tokens no arquivo .txt
    int leafCount;  // numero de folhas na arvore

    unordered_map<string, string> huffmanCodes;

    HuffmanTree(ifstream& file, bool byWord);
    HuffmanTree(vector<pair<string, int>> symbolTable);
    ~HuffmanTree();

    void generateCodes(Node* node, string currentCode);

    // debug
    void showSymbols();
    void showCodes();
    void showTree();
};

