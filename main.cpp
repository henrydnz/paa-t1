#include "huffman.hpp"
#include "arquivoManager.hpp"

int main(){
    Leitor l("letters.txt", "letters.bin");
    l.comprimir(false);

}

