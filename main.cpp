#include "huffman.hpp"
#include "arquivoManager.hpp"

int main(){
    HuffmanTree htWords("words.txt", true);
    writeTree("teste.bin", htWords);
}

