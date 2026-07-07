#include "huffman.hpp"
#include <algorithm>
#include <fstream>
#include <queue>

// === === SymbolTable

// === construtores 

// cria uma tabela de simbolos a partir de uma ifstream de um arquivo .txt e o booleano by word (pra codificacao)
SymbolTable::SymbolTable(ifstream& file, bool byWord){
    if(!byWord){
        char c;
        while(file.get(c)){
            if(!(c == '\n' || c == ' '))
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

// cria uma tabela de simbolos como uma copia de uma outra tabela em forma de vector pair (pra decodificacao)
SymbolTable::SymbolTable(vector<pair<string, int>> symbols) : symbols(symbols){
    if(symbols.empty()) {
        cerr << "O arquivo inserido parece estar vazio!" << endl;
        return;
    }

    tokenCount = 0;
    for(auto sym : symbols) 
        tokenCount += sym.second;
        
    size = symbols.size();
}

// === Metodos

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

// === === HuffmanTree

// === construtores

// constroi arvore a partir do arquivo de texto (pra condificacao)
HuffmanTree::HuffmanTree(ifstream& file, bool byWord) : symbolTable(file, byWord){
    buildTree();
    generateCodes(root, "");
}


// constroi arvore a partir de um vector pair (pra decodificacao)
HuffmanTree::HuffmanTree(vector<pair<string, int>> symbols) : symbolTable(symbols){
    buildTree();
    generateCodes(root, "");
}

// === Metodos

// constroi a arvore a partir de uma fila ordenada fila de nos
void HuffmanTree::buildTree(){
    priority_queue<Node*> nodeQueue;

    for(auto sym : symbolTable.symbols)
        nodeQueue.push(new Node(sym.first, sym.second));

    while(nodeQueue.size() > 1) {
        Node* n1 = nodeQueue.top(); nodeQueue.pop();
        Node* n2 = nodeQueue.top(); nodeQueue.pop();
        
        nodeQueue.push(new Node(n1->count + n2->count, n1, n2));
    }

    root = nodeQueue.top(); nodeQueue.pop();
}

// funcao recursiva pra construir a hash de token->codigoBin (em string)
void HuffmanTree::generateCodes(Node* node, string currentCode) {
    if (!node) return;

    if(node->isLeaf()){
        huffmanCodes[node->token] = currentCode;
        return;
    }

    generateCodes(node->left, currentCode + "0");
    generateCodes(node->right, currentCode + "1");
}

// funcao recursiva pra mostrar nos da arvore
static void printNode(Node* node, unordered_map<string, string> huffmanCodes){
    if(!node) return;

    printNode(node->left,huffmanCodes);
    if(node->token != "") cout << "Symbol: " << node->token << " | " << huffmanCodes[node->token] <<  endl;
    printNode(node->right, huffmanCodes);

}
void HuffmanTree::showTree(){ printNode(root, huffmanCodes); }

// funcao recursiva pro destructor
static void freeNode(Node* node){
    if(!node) return;

    freeNode(node->left);
    freeNode(node->right);

    delete node;
}
HuffmanTree::~HuffmanTree(){ freeNode(root); }



