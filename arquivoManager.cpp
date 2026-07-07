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

static void writeHeader(ofstream& outFile, SymbolTable symbolTable, int& pos){
    int leafCount = symbolTable.size;
    int tokenCount = symbolTable.tokenCount;

    outFile.write(reinterpret_cast<char*>(&leafCount), sizeof(int));
    pos+=sizeof(int);

    outFile.write(reinterpret_cast<char*>(&tokenCount), sizeof(int));
    pos+=sizeof(int);
}

static HuffmanTree getTree(){
    
}
void compress(string filename, bool byWord){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Arquivo de texto " << filename << " não pode ser aberto";
        return;
    }

    HuffmanTree ht(file, byWord);


    
    ofstream outFile(filename, ios::binary);
    if(!outFile){
        cerr << "Nao foi possivel criar o arquivo " << filename << " para compressao.\n";
        return;
    }
    
    
    int pos = 0;
    writeHeader(outFile, ht.symbolTable, pos);
    for(pair symbol : st.symbols)  writeSymbol(outFile, symbol, pos);

    

}