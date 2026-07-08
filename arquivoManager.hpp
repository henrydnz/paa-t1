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
    ~Leitor();
    
};

using namespace std;

void compress(string filename, bool byWord);
