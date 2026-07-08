#include "huffman.hpp"
#include "compressor.hpp"

void Comp_Desc() {
    string inputFile;
    int op;
    bool isWord;
    cout << "Digite o Nome do Arquivo (Sem \".txt\")" << endl;
    cin >> inputFile;
    do {
        cout << "1 - Comprimir por Palavras" << endl
             << "2 - Comprimir por Letras" << endl;
        cin >> op;
        switch(op){
            case 1 : isWord = true; break;
            case 2 : isWord = false; break;
            default : cout << "Erro" << endl;
        }
    } while (op != 1 && op != 2);

    Compressor C(inputFile, isWord);
    C.compress();
    cout << "Arquivo Comprimido!" << endl;
    C.decompress();
    cout << "Arquivo Descomprimido!" << endl;
}

void Comp() {
    string inputFile;
    int op;
    bool isWord;
    cout << "Digite o Nome do Arquivo (Sem \".txt\")" << endl;
    cin >> inputFile;
    do {
        cout << "1 - Comprimir por Palavras" << endl
             << "2 - Comprimir por Letras" << endl;
        cin >> op;
        switch(op){
            case 1 : isWord = true; break;
            case 2 : isWord = false; break;
            default : cout << "Erro" << endl;
        }
    } while (op != 1 && op != 2);

    Compressor C(inputFile, isWord);
    C.compress();
    cout << "Arquivo Comprimido!" << endl;
}

void Desc() {
    string inputFile;
    cout << "Digite o Nome do Arquivo (Sem \".bin\")" << endl;
    cin >> inputFile;

    Compressor C(inputFile, false);
    C.compressedFilename = inputFile + ".bin";
    C.decompress();
    cout << "Arquivo Descomprimido!" << endl;
}

int menuCompressor() {
    int op = -1;
    cout << "==================== Menu ====================" << endl
         << "1 - Comprimir e Descomprimir um Arquivo" << endl
         << "2 - Comprimir um Arquivo" << endl
         << "3 - Descomprimir um Arquivo" << endl
         << "0 - Fechar" << endl
         << "==================== ==== ====================" << endl;
    cin >> op;

    switch(op) {
        case 1 : Comp_Desc(); break;
        case 2 : Comp(); break;
        case 3 : Desc(); break;
        case 0 : return op;
        default : cout << "Opcao invalida" << endl; return op; 
    }

    return op;
}

int main(){
    while(menuCompressor());

    return 0;
}