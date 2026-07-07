#include "arquivoManager.hpp"
#include "huffman.hpp"

Leitor::Leitor(string entrada, string saida) {
    this->entrada = new ifstream(entrada, ios::binary);
    this->saida = new ofstream(saida);
    this->qtdFolhas = lerInteiro();
    this->qtdPalavras = lerInteiro();

    void montarArvore();
}

int Leitor::lerInteiro() {
    int valor;
    entrada->read(reinterpret_cast<char*>(&valor), sizeof(valor));
    return valor;
}

void Leitor::montarArvore() {
    vector<pair<string, int>> listaPalavras;
    string palavra;
    int tamanho;
    int frequencia;
    for(int i = 0; i < qtdFolhas ; i++) {
        tamanho = lerInteiro();
        palavra.resize(tamanho);
        entrada->read((&palavra[0]), tamanho);
        frequencia = lerInteiro();
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
            n = sizeof(char)*8;
        }

        if(ponteiro->leaf()) {
            *saida << ponteiro->token;
            ponteiro = Arvore->root;
        }
        
        if(bucket & 0x80) {
            ponteiro = ponteiro->right;
        }
        else {
            ponteiro = ponteiro->left;
        }
        bucket = bucket << 1; n--;
    }
}