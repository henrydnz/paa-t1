#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp"

using namespace std;

class Leitor {
    private:
        int qtdPalavras;
        int qtdFolhas;
        ifstream* original;
        ofstream* comprimido;
        HuffmanTree* Arvore;

        void montarArvore();
    public:

    void descomprimir();
    void comprimir(bool byWord);

    Leitor(string entrada, string saida);
};

// # Cab_Folhas
// -> Quantidade de Folhas
// -> Quantidade de Palavras
// -> Folha("Palavra","Codigo")
// # fim Cab_Folhas
// Arquivo Compactado


