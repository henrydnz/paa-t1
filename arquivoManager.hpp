#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp"
using namespace std;

typedef int Header;

struct FileNode{
    string token;
    string code;
};

void writeTree(string filename, HuffmanTree tree);

class Leitor {
    private:
        int qtdPalavras;
        int qtdFolhas;
        ifstream* entrada;
        ofstream* saida;
        HuffmanTree* Arvore;

        void montarArvore();
        int lerInteiro();
        char pegarCarimbo();
    public:

    void Descomprimir(string saida);

    Leitor(string entrada);
    ~Leitor();
}

// # Cab_Folhas
// -> Quantidade de Folhas
// -> Quantidade de Palavras
// -> Folha("Palavra","Codigo")
// # fim Cab_Folhas
// Arquivo Compactado