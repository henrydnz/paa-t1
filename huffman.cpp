#include "huffman.hpp"

HuffmanTree::HuffmanTree(ifstream& file, bool byWord) {
    buildSymbolTable(file, byWord);

    NodeQueue nodeQueue;

    for(auto sym : symbolFrequencyTable)
        nodeQueue.push(new Node(sym.first, sym.second));

    while(nodeQueue.size() > 1) {
        Node* n1 = nodeQueue.top(); nodeQueue.pop();
        Node* n2 = nodeQueue.top(); nodeQueue.pop();
        
        nodeQueue.push(new Node(n1->count + n2->count, n1, n2));
    }

    root = nodeQueue.top(); nodeQueue.pop();
    generateCodes(root, "");
}

HuffmanTree::HuffmanTree(vector<pair<string, int>> symbolList){
    NodeQueue nodeQueue;

    for(auto sym : symbolList)
        nodeQueue.push(new Node(sym.first, sym.second));

    while(nodeQueue.size() > 1) {
        Node* n1 = nodeQueue.top(); nodeQueue.pop();
        Node* n2 = nodeQueue.top(); nodeQueue.pop();
        
        nodeQueue.push(new Node(n1->count + n2->count, n1, n2));
    }

    root = nodeQueue.top(); nodeQueue.pop();
}

static void freeNode(Node* node){
    if(!node) return;

    freeNode(node->left);
    freeNode(node->right);

    delete node;
}
HuffmanTree::~HuffmanTree(){ freeNode(root); }

void HuffmanTree::buildSymbolTable(ifstream& file, bool byWord){

    char c;

    if(!byWord) {
        while(file.get(c)) symbolFrequencyTable[string(1, c)]++;
    } else {
        string word = "";
        while(file.get(c)) {
            if(c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                if(!word.empty()) {
                    symbolFrequencyTable[word]++;
                    word = "";
                }
                symbolFrequencyTable[string(1, c)]++;
            } else word += c;
        }
        if(!word.empty()) symbolFrequencyTable[word]++;
    }
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



