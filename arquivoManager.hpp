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
        ifstream* entrada;
        ofstream* saida;
        HuffmanTree* Arvore;

        void montarArvore();
    public:

    void descomprimir();
    void comprimir(bool byWord);

    Leitor(string entrada, string saida);
    ~Leitor();
    
};

using namespace std;


