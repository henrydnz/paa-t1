#include "huffman.hpp"
#include "arquivoManager.hpp"

int main(){
    Compressor c1("letters.txt", "letters.bin", "Letras.txt", false);
    c1.compress();
    c1.decompress();

    Compressor c2("texto.txt", "texto.bin", "Palavras.txt", true);
    c2.compress();
    c2.decompress();
}