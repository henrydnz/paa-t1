#include "huffman.hpp"
#include <algorithm>
#include <fstream>
#include <queue>


SymbolTable::SymbolTable(string filename, bool byWord){
    ifstream file(filename);

    if(!file.is_open()) {
        cerr << "Nao conseguiu abrir o arquivo (" << filename << ")." << endl;
        return;
    }

    if(!byWord){
        char c;
        while(file.get(c)){
            this->addSymbol(string(1,c));
        }
    } else {
        string word;
        while(file >> word){
            this->addSymbol(word);
        }
    }

    file.close();

    if(symbols.empty()) {
        cerr << "O arquivo inserido parece estar vazio!" << endl;
        return;
    }

    tokenCount = 0;
    for(auto sym : symbols) 
        tokenCount += sym.second;
        
    size = symbols.size();
}

void SymbolTable::addSymbol(string str){
    for(auto& sym : symbols){
        if(sym.first == str){
            sym.second++; return;
        }
    }
    symbols.push_back(make_pair(str, 1));
}

void SymbolTable::showTable(){
    for(auto sym : symbols){ cout << "symbol " << sym.first << ": " << sym.second << endl; }
}

void HuffmanTree::getQueue(){
    for(auto sym : symbolTable.symbols){
        string token = sym.first;
        long long counter = sym.second;

        Node* node = new Node(token, nullptr, nullptr);
        queue.push(NodeWrapper(node, counter)); 
    }
}

void HuffmanTree::buildTree(){
    while(queue.size() > 1) {
        NodeWrapper w1 = queue.top(); queue.pop();
        NodeWrapper w2 = queue.top(); queue.pop();
    
        Node* n1 = w1.node;
        Node* n2 = w2.node;
        
        Node* parent_node = new Node("", n1, n2);
        NodeWrapper parent_nw(parent_node, w1.count + w2.count);

        queue.push(parent_nw);
    }

    NodeWrapper root_nw = queue.top(); queue.pop();
    root = root_nw.node;
}

HuffmanTree::HuffmanTree(string filename, bool byWord) : symbolTable(filename, byWord){
    getQueue();
    buildTree();
    generateCodes(root, "");
}

// debug

void HuffmanTree::generateCodes(Node* node, string currentCode) {
    if (!node) return;

    if(node->isLeaf()){
        huffmanCodes[node->token] = currentCode;
        return;
    }

    generateCodes(node->left, currentCode + "0");
    generateCodes(node->right, currentCode + "1");
}

static void printNode(Node* node, unordered_map<string, string> huffmanCodes){
    if(!node) return;

    printNode(node->left,huffmanCodes);
    if(node->token != "") cout << "Symbol: " << node->token << " | " << huffmanCodes[node->token] <<  endl;
    printNode(node->right, huffmanCodes);
}

// void HuffmanTree::showTree(){ printNode(root, huffmanCodes); }

static void freeNode(Node* node){
    if(!node) return;

    freeNode(node->left);
    freeNode(node->right);

    delete node;
}

HuffmanTree::~HuffmanTree(){ freeNode(root); }



