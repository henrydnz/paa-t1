#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "huffman.hpp"

using namespace std;

/**
 * @class Compressor
 * @brief Gerencia o processo de compressão e descompressão Lossless.
 */
class Compressor{
    private:

    bool byWord;
    Node* treeRoot;
    int leafCount, tokenCount;

    vector<Symbol> symbols;

    void writeCompressedMessage(ifstream& originalFile, ofstream& compressedFile, HuffmanCodes codes);
    void writeDecompressedMessage(ifstream& compressedFile, ofstream& decompressedFile);
    void getSymbols(ifstream& compressedFile);
    
    public:
    
    string originalFilename, compressedFilename, outputFilename;

    /**
     * @brief Construtor da classe Compressor.
     * @param originalFilename Nome base do arquivo de texto original (sem o .txt).
     * @param byWord Define o método de compressão (true para Palavras, false para Letras).
     */
    Compressor(string originalFilename, bool byWord);

    /**
     * @brief Comprime o arquivo original em um arquivo binário (.bin).
     * 
     * Executa a leitura do arquivo original duas vezes: a primeira para montar a tabela
     * Hash de códigos de Huffman e gravar o cabeçalho (quantidade de folhas e tokens), e a 
     * segunda para gravar a cadeia crua de bits correspondente à mensagem codificada.
     */
    void compress();

    /**
     * @brief Descomprime o arquivo binário gerando um arquivo de texto.
     * 
     * Lê inicialmente as quantidades de folhas e tokens para recriar a lista de relação
     * e reconstruir a árvore de Huffman idêntica à da compressão. Em seguida, 
     * navega a árvore bit a bit decodificando a mensagem e gravando os tokens recuperados.
     */
    void decompress();
};