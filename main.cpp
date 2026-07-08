#include "huffman.hpp"
#include "arquivoManager.hpp"

int main(){
    Compressor c("letters.txt", "letters.bin", false);
    c.decompress();
}

