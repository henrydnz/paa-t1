#include "util.hpp"
using namespace std;

void clearScreen(){
    #if defined(_WIN32)
        system("cls");   
    #else
        system("clear"); 
    #endif
}

void showMenu(){
    cout << "==================== Menu ====================" << endl
        << "1 - Comprimir e Descomprimir um Arquivo" << endl
        << "2 - Comprimir um Arquivo" << endl
        << "3 - Descomprimir um Arquivo" << endl
        << "0 - Fechar" << endl
        << "==================== ==== ====================" << endl;
}

string timeElapsed(string filename, string action, chrono::_V2::steady_clock::time_point start, chrono::_V2::steady_clock::time_point end){
    return "Arquivo " + filename + " foi " + action + " com sucesso! (Tempo:" + 
        to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms)\n";
}

string fileSize(string filename, string type, streampos filePointer){ 
    return type + ": " + filename + " (" + 
        to_string(static_cast<double>(filePointer)/1024.0) + "kb)\n\n"; 
}