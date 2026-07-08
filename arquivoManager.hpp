#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp"

using namespace std;

class Compressor{
    private:

    string decompressedFilename;
    string compressedFilename;
    string outputFilename;

    bool byWord;

    HuffmanTree* huffmanTree;

    public:

    Compressor(string decompressedFilename, bool byWord);

    void decompress();
    void compress();
};
// # Cab_Folhas
// -> Quantidade de Folhas
// -> Quantidade de Palavras
// -> Folha("Palavra","Codigo")
// # fim Cab_Folhas
// Arquivo Compactado


