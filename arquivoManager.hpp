#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp"

class Leitor {
    private:
        int qtdPalavras;
        int qtdFolhas;
        ifstream* entrada;
        ofstream* saida;
        HuffmanTree* Arvore;

        void montarArvore();
    public:

    void Descomprimir();

    Leitor(string entrada, string saida);
    
}

// # Cab_Folhas
// -> Quantidade de Folhas
// -> Quantidade de Palavras
// -> Folha("Palavra","Codigo")
// # fim Cab_Folhas
// Arquivo Compactado
using namespace std;

void compress(string filename, bool byWord);
