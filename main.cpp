#include "huffman.hpp"
#include "arquivoManager.hpp"

int main(){
    Compressor c1("TesteLetrasMenor", false);
    c1.compress();
    c1.decompress();

    Compressor c2("TestePalavras", true);
    c2.compress();
    c2.decompress();
}