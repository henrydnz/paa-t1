#include "arquivoManager.hpp"
#include "huffman.hpp"

static void writeSymbol(ofstream& outFile, pair<string, int> symbol, int& pos){
    string token = symbol.first;
    int tokenLen = token.length();
    int frequency = symbol.second;

    outFile.seekp(pos, ios::beg);

    outFile.write(reinterpret_cast<char*>(&tokenLen), sizeof(int));
    pos += sizeof(int);

    outFile.write(reinterpret_cast<char*>(&token), sizeof(token));
    pos += sizeof(token);

    outFile.write(reinterpret_cast<char*>(&frequency), sizeof(int));

}

static void writeSymbolTable(ofstream& outFile, SymbolTable st, int& pos){
    for(pair sym : st.symbols){
        string token = sym.first;
        size_t tokenSize = sizeof(token);
        int frequency = sym.second;

        outFile.seekp(pos, ios::beg);
        outFile.write(reinterpret_cast<char*>(&token), tokenSize);
        pos += tokenSize;
        outFile.write(reinterpret_cast<char*>(&frequency), intSize);
        pos += intSize;
    }
}

static void writeHeader(ofstream& outFile, SymbolTable symbolTable, int& pos){
    int leafCount = symbolTable.size;
    int tokenCount = symbolTable.tokenCount;

    outFile.write(reinterpret_cast<char*>(&leafCount), sizeof(int));
    pos+=sizeof(int);

    outFile.write(reinterpret_cast<char*>(&tokenCount), sizeof(int));
    pos+=sizeof(int);
}

void compress(string filename, HuffmanTree ht){
    ofstream outFile(filename, ios::binary);

    if(!outFile){
        cerr << "Nao foi possivel criar o arquivo " << filename << ".\n";
        return;
    }

    int pos = 0;

    writeHeader(outFile, ht.symbolTable, pos);
    writeSymbolTable(outFile, ht.symbolTable, pos);


}