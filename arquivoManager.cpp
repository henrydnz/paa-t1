#include "arquivoManager.hpp"
#include "huffman.hpp"

Leitor::Leitor(string entrada, string saida) {
    this->entrada = new ifstream(entrada, ios::binary);
    this->saida = new ofstream(saida);
    this->entrada->read(reinterpret_cast<char*>(&qtdFolhas), sizeof(qtdFolhas));
    this->entrada->read(reinterpret_cast<char*>(&qtdPalavras), sizeof(qtdPalavras));

    void montarArvore();
}

void Leitor::montarArvore() {
    vecor<pair<string, int>> listaPalavras;
    string palavra;
    int tamanho;
    int frequencia;
    for(int i = 0; i < qtdFolhas ; i++) {
        entrada->read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));
        palavra.resize(tamanho);
        entrada->read((&palavra[0]), tamanho);
        entrada->read(reinterpret_cast<char*>(&frequencia), sizeof(frequencia));
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

        if(ponteiro->leaf()) {
            *saida << ponteiro->token;
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

void Leitor::comprimir(string filename, bool byWord){
    ifstream inFile(filename+".txt");

    if(!inFile.is_open()){
        cerr << "Arquivo de texto " << filename+".txt" << " não pode ser aberto";
        return;
    }

    HuffmanTree ht(inFile, byWord);
    inFile.close();

    // DEBUG
    ht.showTree();

    
    ofstream outFile(filename+".bin", ios::binary);

    if(!outFile){
        cerr << "Nao foi possivel criar o arquivo " << filename << " para compressao.\n";
        return;
    }
    
    int pos = 0;

    writeHeader(outFile, ht, pos);

    for(pair symbol : ht.symbols)  
        writeSymbol(outFile, symbol, pos);

    // escerever mensagem codificada
    
    //outFile.close();
}