#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "huffman.hpp"

using namespace std;

class Compressor{
    private:

    
    bool byWord;
    
    HuffmanTree* huffmanTree;
    
    public:
    
    string originalFilename;
    string compressedFilename;
    string outputFilename;

    Compressor(string originalFilename, bool byWord);

    void decompress();
    void compress();
};
// # Cab_Folhas
// -> Quantidade de Folhas
// -> Quantidade de Palavras
// -> Folha("Palavra","Codigo")
// # fim Cab_Folhas
// Arquivo Compactado


