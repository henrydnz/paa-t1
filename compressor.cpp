#include "compressor.hpp"
#include <chrono>

Compressor::Compressor(string originalFilename, bool byWord) {
    this->originalFilename = originalFilename + ".txt";
    this->compressedFilename = originalFilename + "_binary.bin";
    this->outputFilename = originalFilename + "_output.txt";
    this->byWord = byWord;
}

static void writeSymbols(ofstream& compressedFile, vector<Symbol> symbols){
    for(Symbol symbol : symbols){
        string token = symbol.first;
        int tokenLen = symbol.first.length();
        int frequency = symbol.second;

        compressedFile.write(reinterpret_cast<char*>(&tokenLen), sizeof(int));

        compressedFile.write(token.c_str(), tokenLen);

        compressedFile.write(reinterpret_cast<char*>(&frequency), sizeof(int));
    }
}

static void writeBitBuffer(ofstream& compressedFile, char& buffer, int& bitCount, string code){
    for (char c : code) {
        buffer = (buffer << 1) | (c == '1' ? 1 : 0);
        bitCount++;
        if (bitCount == 8) {
            compressedFile.write(reinterpret_cast<const char*>(&buffer), 1);
            buffer = 0;
            bitCount = 0;
        }
    }
}

static void writeCompressedMessage(ifstream& originalFile, ofstream& compressedFile, unordered_map<string, string> codes, bool byWord){
    originalFile.clear();
    originalFile.seekg(0, ios::beg);

    char buffer = 0;
    int bitCount = 0;
    char c;
    if (!byWord) {
        while (originalFile.get(c)) {
            string code = codes[string(1, c)];
            writeBitBuffer(compressedFile, buffer, bitCount, code);
        }
    } else {
        string word = "";
        while (originalFile.get(c)) {
            if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                if (!word.empty()) {
                    string code = codes[word];
                    writeBitBuffer(compressedFile, buffer, bitCount, code);
                    word = "";
                }
                string spaceCode = codes[string(1, c)];
                writeBitBuffer(compressedFile, buffer, bitCount, spaceCode);
                
            } else word += c;
        }
        if (!word.empty()) {
            string code = codes[word];
            writeBitBuffer(compressedFile, buffer, bitCount, code);
        }
    }

    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        compressedFile.write(reinterpret_cast<const char*>(&buffer), 1);
    } 
}

void Compressor::compress(){
    // auto start = chrono::steady_clock::now();

    ifstream originalFile(originalFilename);
    HuffmanTree ht(originalFile, byWord);
    
    ofstream compressedFile(compressedFilename, ios::binary);

    compressedFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    compressedFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));

    writeSymbols(compressedFile, ht.symbols);
    writeCompressedMessage(originalFile, compressedFile, ht.huffmanCodes, byWord);

    originalFile.close();
    compressedFile.close();

    // auto end = chrono::steady_clock::now();
    // auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    // cout << "Tempo de compactacao de arquivo " << originalFilename << (byWord ? " por palavra: " : " por letra: ") << duration.count() << "ms\n";
}

static vector<Symbol> getSymbols(ifstream& compressedFile, int leafCount){
    vector<Symbol> symbols;
    string symbol;
    int size, frequency;
    for(int i = 0; i < leafCount ; i++) {
        compressedFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        symbol.resize(size);

        compressedFile.read((&symbol[0]), size);

        compressedFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));

        symbols.push_back(make_pair(symbol, frequency));
    }

    return symbols;
}

static void writeDecompressedMessage(ifstream& compressedFile, ofstream& decompressedFile, int tokenCount, Node* htRoot){
    char buffer;
    int bitCount = 0;
    int foundTokens = 0;
    Node* currentNode = htRoot;

    while(foundTokens < tokenCount) {
        if(bitCount == 0) {
            buffer = compressedFile.get();
            if (compressedFile.eof()) {cerr << "EOF!" << endl; break;}
            bitCount = 8;
        }

        currentNode = (buffer & 0x80) ? currentNode->right : currentNode->left;

        buffer = buffer << 1; 
        bitCount--;

        if(currentNode->isLeaf()) {
            decompressedFile << currentNode->token;
            currentNode = htRoot;
            foundTokens++;
        }
    }
}

void Compressor::decompress(){
    // auto start = chrono::steady_clock::now();

    ifstream compressedFile(compressedFilename, ios::binary);

    int leafCount, tokenCount;

    compressedFile.read(reinterpret_cast<char*>(&leafCount), sizeof(int));
    compressedFile.read(reinterpret_cast<char*>(&tokenCount), sizeof(int));
    
    vector<Symbol> symbols = getSymbols(compressedFile, leafCount);

    HuffmanTree ht(symbols);

    Node* htRoot = ht.root;

    ofstream decompressedFile(outputFilename);

    writeDecompressedMessage(compressedFile, decompressedFile, tokenCount, htRoot);

    compressedFile.close();
    decompressedFile.close();
    
    // auto end = chrono::steady_clock::now();
    // auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    // cout << "Tempo de descompactacao de arquivo " << compressedFilename << (byWord ? " por palavra: " : " por letra: ") << duration.count()  << "ms\n";

}