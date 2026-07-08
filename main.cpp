#include "huffman.hpp"
#include "arquivoManager.hpp"

using namespace std;

int main() {
    cout << "=== INICIO ===" << endl;
    
    // compress("texto", true);
    // cout << "=== DEPOIS DA COMPRESSÃO ===" << endl;
    
    Leitor leitor("texto.bin", "descomprimido.txt");
    cout << "=== DEPOIS DO CONSTRUTOR ===" << endl;
    
    leitor.descomprimir();
    cout << "=== DEPOIS DA DESCOMPRESSAO ===" << endl;
    
    return 0;
}