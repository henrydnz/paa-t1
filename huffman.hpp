#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

/**
 * @class Node
 * @brief Estrutura do nó da árvore binária de Huffman.
 */
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

/**
 * @struct NodePointerComparator
 * @brief Estrutura para comparação de ponteiros de no, usada na priority_queue<>
 */
struct NodePointerComparator {
    bool operator()(Node* n1, Node* n2){ return (*n1) < (*n2); }
};

typedef priority_queue<Node*, vector<Node*>, NodePointerComparator> NodeQueue;
typedef pair<string, int> Symbol;
typedef unordered_map<string, string> HuffmanCodes;

/**
 * @class HuffmanTree
 * @brief Classe responsável pela construção e gestão da Árvore de Huffman.
 * 
 * A árvore é construída a partir de uma lista de relação transformada em nós-folha,
 * processada por uma fila de prioridade (priority_queue) baseada em frequência.
 * A partir da árvore, gera-se uma tabela Hash (unordered_map) contendo o código binário de cada token.
 */
class HuffmanTree{    
    private:

    // adiciona um simbolo na tabela
    void addSymbol(string symbol);
    // constroi tabela de simbolos
    void buildSymbolTable(ifstream& file, bool byWord);
    // constroi arvore com base na tabela de simbolos
    void buildTree();
    // gera hash de codigos Huffman recursivamente
    void generateCodes(Node* node, string currentCode);

    public:

    Node* root = nullptr;

    vector<Symbol> symbols;
    int tokenCount; // numero de tokens no arquivo .txt
    int leafCount;  // numero de folhas na arvore

    HuffmanCodes huffmanCodes;

    /**
     * @brief Constrói a árvore de Huffman para COMPRESSÃO lendo o arquivo original.
     * @param file Arquivo original aberto em modo binário.
     * @param byWord Se true, agrupa caracteres formando palavras; se false, lê caractere por caractere.
     */
    HuffmanTree(ifstream& file, bool byWord);

    /**
     * @brief Constrói a árvore de Huffman para DESCOMPRESSÃO a partir de símbolos lidos do arquivo .bin.
     * @param symbolTable Vetor de pares contendo o token e sua respectiva frequência.
     */
    HuffmanTree(vector<Symbol> symbolTable);

    ~HuffmanTree();
};

