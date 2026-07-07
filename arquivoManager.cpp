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

void Leitor::Descomprimir(string saida) {
    char carimbo;
    int n = sizeof(char);
    Node* ponteiro = Arvore->root;
    for(int i = 0; i < qtdPalavras; i++) {
        
    }
}