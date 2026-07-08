#include "huffman.hpp"
#include "arquivoManager.hpp"

int main(){
    Compressor c1("letters.txt", "letters.bin", false);
    c1.compress();

    Compressor c2("words.txt", "words.bin", true);
    c2.compress();
}

