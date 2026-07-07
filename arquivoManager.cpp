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
    vector<pair<string, int>> listaPalavras;
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

void Leitor::Descomprimir() {
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

Leitor::~Leitor() {

}