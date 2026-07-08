#include "arquivoManager.hpp"
#include "huffman.hpp"

Leitor::Leitor(string entrada, string saida) {
    cout << "testando\n";
    this->entrada = new ifstream(entrada, ios::binary);
    this->saida = new ofstream(saida);
    this->entrada->read(reinterpret_cast<char*>(&qtdFolhas), sizeof(qtdFolhas));
    cout << this->qtdFolhas << endl;
    this->entrada->read(reinterpret_cast<char*>(&qtdPalavras), sizeof(qtdPalavras));
    cout << this->qtdPalavras << endl;

    montarArvore();
}

void Leitor::montarArvore() {
    vector<pair<string, int>> listaPalavras;
    string palavra;
    int tamanho;
    int frequencia;
    for(int i = 0; i < qtdFolhas ; i++) {
        entrada->read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));
        palavra.resize(tamanho);
        entrada->read((&palavra[0]), tamanho);
        entrada->read(reinterpret_cast<char*>(&frequencia), sizeof(frequencia));
        cout << palavra << " " << frequencia << endl;
        listaPalavras.push_back(make_pair(palavra, frequencia));
    }

    this->Arvore = new HuffmanTree(listaPalavras);
}

void Leitor::descomprimir() {
    char bucket;
    int n = 0;
    Node* ponteiro = Arvore->root;
    for(int i = 0; i < qtdPalavras; i++) {

        if(n == 0) {
            bucket = entrada->get();
            if (entrada->eof()) {cerr << "EOF!" << endl; break;}
            n = 8;
        }
        
        if(bucket & 0x80) {
            ponteiro = ponteiro->right;
        }

        else {
            ponteiro = ponteiro->left;
        }
        bucket = bucket << 1; n--;

        if(ponteiro->isLeaf()) {
            *saida << ponteiro->token;
            ponteiro = Arvore->root;
        }
    }
}

Leitor::~Leitor() {
    cout << "Acho q sim";
}

static void writeSymbol(ofstream& outFile, pair<string, int> symbol, int& pos){
    string token = symbol.first;
    int tokenLen = token.length();
    int frequency = symbol.second;

    outFile.seekp(pos, ios::beg);

    outFile.write(reinterpret_cast<char*>(&tokenLen), sizeof(int));
    pos += sizeof(int);

    outFile.write(token.c_str(), token.length());
    pos += token.length();

    outFile.write(reinterpret_cast<char*>(&frequency), sizeof(int));
    pos += sizeof(int);
}

static void writeHeader(ofstream& outFile, HuffmanTree ht, int& pos){
    outFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    pos+=sizeof(int);

    outFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));
    pos+=sizeof(int);
}

void Leitor::comprimir(bool byWord){
    

    HuffmanTree ht(*this->entrada, byWord);

    // DEBUG
    ht.showTree();
    
    int pos = 0;

    writeHeader(*this->saida, ht, pos);

    // for(pair<string, int> symbol : ht.symbols)  
    //    writeSymbol(outFile, symbol, pos);
    
    // outFile.close();
}