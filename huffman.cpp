#include "huffman.hpp"

// === === HuffmanTree

// === construtores

// constroi arvore a partir do arquivo de texto (pra condificacao)
HuffmanTree::HuffmanTree(ifstream& file, bool byWord) {
    buildSymbolTable(file, byWord);

    tokenCount = 0;
    for(auto sym : symbols) 
        tokenCount += sym.second;

    leafCount = symbols.size();

    buildTree();
    generateCodes(root, "");
}


// constroi arvore a partir de um vector pair (pra decodificacao)
HuffmanTree::HuffmanTree(vector<pair<string, int>> symbols) : symbols(symbols) {
    leafCount = symbols.size();
    tokenCount = 0;
    for(auto sym : symbols) 
        tokenCount += sym.second;

    buildTree();
    generateCodes(root, "");
}

// === Metodos
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

// constroi a arvore a partir de uma fila ordenada fila de nos
void HuffmanTree::buildTree(){
    struct CompareNode {
        bool operator()(Node* n1, Node* n2){
            return n1->count > n2->count; 
        }
    };
    priority_queue<Node*, vector<Node*>, CompareNode> nodeQueue;

    for(auto sym : symbols)
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

void HuffmanTree::showSymbols(){
    cout << "showing symbols and frequencies for the current tree" << endl;
    for(pair<string, int> symbol : symbols){
        cout << "Symbol (" << symbol.first << ") : " << symbol.second << endl;
    }
}

void HuffmanTree::showCodes(){
    cout << "showing huffman codes for the current tree" << endl;
    for(auto symbol : huffmanCodes){
        cout << "Symbol (" << symbol.first << ") : " << symbol.second << endl;
    }
}

// funcao recursiva pra mostrar nos da arvore
static void printNode(Node* node, const unordered_map<string, string>& huffmanCodes){
    if(!node) return;

    printNode(node->left,huffmanCodes);

    if(node->token != "") {
        cout << "Symbol (" << node->token << ")" << endl;
        cout << "    code: " << huffmanCodes.at(node->token) <<  endl;
        cout << "    frequency: " << node->count << endl;
    }

    printNode(node->right, huffmanCodes);

}

void HuffmanTree::showTree(){ 
    cout << "showing current tree" << endl;
    printNode(root, huffmanCodes); 
}

// funcao recursiva pro destructor
static void freeNode(Node* node){
    if(!node) return;

    freeNode(node->left);
    freeNode(node->right);

    delete node;
}
HuffmanTree::~HuffmanTree(){ freeNode(root); }



