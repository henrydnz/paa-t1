#include "huffman.hpp"

HuffmanTree::HuffmanTree(ifstream& file, bool byWord) {
    buildSymbolTable(file, byWord);

    tokenCount = 0;
    for(auto sym : symbols) 
        tokenCount += sym.second;

    leafCount = symbols.size();

    buildTree();
    generateCodes(root, "");
}

HuffmanTree::HuffmanTree(vector<Symbol> symbols) : symbols(symbols) {
    leafCount = symbols.size();
    tokenCount = 0;
    for(auto sym : symbols) 
        tokenCount += sym.second;

    buildTree();
    generateCodes(root, "");
}

static void freeNode(Node* node){
    if(!node) return;

    freeNode(node->left);
    freeNode(node->right);

    delete node;
}
HuffmanTree::~HuffmanTree(){ freeNode(root); }

void HuffmanTree::addSymbol(string symbol){
    for(auto& sym : symbols){
        if(sym.first == symbol){
            sym.second++; return;
        }
    }
    symbols.push_back(make_pair(symbol, 1));
}

void HuffmanTree::buildSymbolTable(ifstream& file, bool byWord){
    char c;
    if(!byWord){
        while(file.get(c)) this->addSymbol(string(1,c));
    } else {
        string word = "";
        while(file.get(c)) {
            if(c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                if(!word.empty()) {
                    this->addSymbol(word);
                    word = "";
                }
                this->addSymbol(string(1, c));
            } else word += c;
        }
        if(!word.empty()) this->addSymbol(word);
    }
}

void HuffmanTree::buildTree(){
    NodeQueue nodeQueue;

    for(auto sym : symbols)
        nodeQueue.push(new Node(sym.first, sym.second));

    while(nodeQueue.size() > 1) {
        Node* n1 = nodeQueue.top(); nodeQueue.pop();
        Node* n2 = nodeQueue.top(); nodeQueue.pop();
        
        nodeQueue.push(new Node(n1->count + n2->count, n1, n2));
    }

    root = nodeQueue.top(); nodeQueue.pop();
}

void HuffmanTree::generateCodes(Node* node, string currentCode) {
    if (!node) return;

    if(node->isLeaf()){
        huffmanCodes[node->token] = currentCode;
        return;
    }

    generateCodes(node->left, currentCode + "0");
    generateCodes(node->right, currentCode + "1");
}



