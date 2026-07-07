#include "arquivoManager.hpp"
#include "huffman.hpp"

Leitor::Leitor(string original, string comprimido) {
    this->original = new ifstream(original, ios::binary);
    if(!(*this->original).is_open()){
        cerr << "Arquivo de texto " << original << " não pode ser aberto";
        return;
    }

    this->comprimido = new ofstream(comprimido);
    if(!(*this->comprimido)){
        cerr << "Nao foi possivel criar o arquivo " << comprimido << " para compressao.\n";
        return;
    }

    this->original->read(reinterpret_cast<char*>(&qtdFolhas), sizeof(qtdFolhas));
    this->original->read(reinterpret_cast<char*>(&qtdPalavras), sizeof(qtdPalavras));

    void montarArvore();
}

void Leitor::montarArvore() {
    vector<pair<string, int>> listaPalavras;
    string palavra;
    int tamanho;
    int frequencia;
    for(int i = 0; i < qtdFolhas ; i++) {
        original->read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));
        palavra.resize(tamanho);
        original->read((&palavra[0]), tamanho);
        original->read(reinterpret_cast<char*>(&frequencia), sizeof(frequencia));
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
            bucket = original->get();
            if (original->eof()) {cerr << "EOF!" << endl; break;}
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
            *comprimido << ponteiro->token;
            ponteiro = Arvore->root;
        }
    }
}

//Leitor::~Leitor()

static void writeSymbol(ofstream& outFile, pair<string, int> symbol, int& pos){
    string token = symbol.first;
    int tokenLen = token.length();
    int frequency = symbol.second;

    outFile.seekp(pos, ios::beg);

    outFile.write(reinterpret_cast<char*>(&tokenLen), sizeof(int));
    pos += sizeof(int);

    outFile.write(reinterpret_cast<char*>(&token), sizeof(token));
    pos += sizeof(token);

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
    

    HuffmanTree ht(*this->original, byWord);

    // DEBUG
    ht.showTree();
    
    int pos = 0;

    writeHeader(*this->comprimido, ht, pos);

    for(pair symbol : ht.symbols)  
        writeSymbol(*this->comprimido, symbol, pos);

    // escerever mensagem codificada
    
    //outFile.close();
}