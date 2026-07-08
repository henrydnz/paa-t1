#pragma once

#include <cstdlib>
#include <iostream>
using namespace std;

void clear(){
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