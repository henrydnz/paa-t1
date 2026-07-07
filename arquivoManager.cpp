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
    pos += sizeof(int);
}

static void writeHeader(ofstream& outFile, HuffmanTree ht, int& pos){
    outFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    pos+=sizeof(int);

    outFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));
    pos+=sizeof(int);
}

void compress(string filename, bool byWord){
    ifstream inFile(filename+".txt");

    if(!inFile.is_open()){
        cerr << "Arquivo de texto " << filename+".txt" << " não pode ser aberto";
        return;
    }

    HuffmanTree ht(inFile, byWord);
    inFile.close();

    // DEBUG
    ht.showTree();

    
    ofstream outFile(filename+".bin", ios::binary);

    if(!outFile){
        cerr << "Nao foi possivel criar o arquivo " << filename << " para compressao.\n";
        return;
    }
    
    int pos = 0;

    writeHeader(outFile, ht, pos);

    for(pair symbol : ht.symbols)  
        writeSymbol(outFile, symbol, pos);
    
    //outFile.close();
}