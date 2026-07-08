#include "huffman.hpp"
#include "compressor.hpp"
#include "util.hpp"

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

    clear();

    Compressor C(inputFile, isWord);
    int error = C.compress();
    if(!error) C.decompress();
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

    clear();

    Compressor C(inputFile, isWord);
    C.compress();
}

void Desc() {
    string inputFile;
    cout << "Digite o Nome do Arquivo (Sem \".bin\")" << endl;
    cin >> inputFile;

    clear();

    Compressor C(inputFile, false);
    C.compressedFilename = inputFile + ".bin";
    C.decompress();
}

int menuCompressor() {
    int op = -1;
    showMenu();
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