#include "compressor.hpp"
#include <chrono>

// static HuffmanTree getTree(ifstream& inFile, int leafCount){
//     vector<pair<string, int>> symbols;
//     string symbol;
//     int size;
//     int frequency;
//     for(int i = 0; i < leafCount ; i++) {
//         inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
//         symbol.resize(size);
//         inFile.read((&symbol[0]), size);
//         inFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
//         symbols.push_back(make_pair(symbol, frequency));
//     }

//     return HuffmanTree(symbols);
// }

static vector<pair<string, int>> getSymbols(ifstream& inFile, int leafCount){
    vector<pair<string, int>> symbols;
    string symbol;
    int size;
    int frequency;
    for(int i = 0; i < leafCount ; i++) {
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        symbol.resize(size);
        inFile.read((&symbol[0]), size);
        inFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
        symbols.push_back(make_pair(symbol, frequency));
    }

    return symbols;
}

Compressor::Compressor(string decompressedFilename, bool byWord) {
    this->decompressedFilename = decompressedFilename + ".txt";
    this->compressedFilename = decompressedFilename + "_binary.bin";
    this->outputFilename = decompressedFilename + "_output.txt";
    this->byWord = byWord;
}

void Compressor::decompress(){
    auto start = chrono::steady_clock::now();

    ifstream compressed(compressedFilename, ios::binary);
    int leafCount, tokenCount;
    compressed.read(reinterpret_cast<char*>(&leafCount), sizeof(int));
    compressed.read(reinterpret_cast<char*>(&tokenCount), sizeof(int));
    
    //HuffmanTree ht = getTree(compressed, leafCount);
    vector<pair<string, int>> symbols = getSymbols(compressed, leafCount);
    HuffmanTree ht(symbols);

    ofstream decompressed(outputFilename);

    char buffer;
    int bitCount = 0;
    int foundTokens = 0;
    Node* currentNode = ht.root;

    while(foundTokens < tokenCount) {
        if(bitCount == 0) {
            buffer = compressed.get();
            if (compressed.eof()) {cerr << "EOF!" << endl; break;}
            bitCount = 8;
        }

        currentNode = (buffer & 0x80) ? currentNode->right : currentNode->left;
        buffer = buffer << 1; bitCount--;

        if(currentNode->isLeaf()) {
            decompressed << currentNode->token;
            currentNode = ht.root;
            foundTokens++;
        }
    }

    auto end = chrono::steady_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Tempo de descompactacao de arquivo " << compressedFilename << (byWord ? " por palavra: " : " por letra: ") << duration.count()  << "ms\n";
}

static void writeSymbol(ofstream& outFile, pair<string, int> symbol){
    string token = symbol.first;
    int tokenLen = symbol.first.length();
    int frequency = symbol.second;

    outFile.write(reinterpret_cast<char*>(&tokenLen), sizeof(int));
    outFile.write(token.c_str(), tokenLen);
    outFile.write(reinterpret_cast<char*>(&frequency), sizeof(int));
}

static void writeHeader(ofstream& outFile, HuffmanTree ht, int& pos){
    outFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    pos+=sizeof(int);

    outFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));
    pos+=sizeof(int);
}

static void writeBuffer(ofstream& outFile, char& buffer, int& bitCount, string code){
    for (char c : code) {
        buffer = (buffer << 1) | (c == '1' ? 1 : 0);
        bitCount++;
        if (bitCount == 8) {
            outFile.write(reinterpret_cast<const char*>(&buffer), 1);
            buffer = 0;
            bitCount = 0;
        }
    }
}

void Compressor::compress(){
    auto start = chrono::steady_clock::now();

    ifstream inFile(decompressedFilename);
    HuffmanTree ht(inFile, byWord);
    
    ofstream outFile(compressedFilename, ios::binary);

    outFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    outFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));

    for(pair<string, int> symbol : ht.symbols)  
        writeSymbol(outFile, symbol);

    inFile.clear();
    inFile.seekg(0, ios::beg);

    char buffer = 0;
    int bitCount = 0;
    char c;
    if (!byWord) {
        while (inFile.get(c)) {
            string code = ht.huffmanCodes[string(1, c)];
            writeBuffer(outFile, buffer, bitCount, code);
        }
    } else {
        string word = "";
        while (inFile.get(c)) {
            if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
                if (!word.empty()) {
                    string code = ht.huffmanCodes[word];
                    writeBuffer(outFile, buffer, bitCount, code);
                    word = "";
                }
                string spaceCode = ht.huffmanCodes[string(1, c)];
                writeBuffer(outFile, buffer, bitCount, spaceCode);
                
            } else word += c;
        }
        if (!word.empty()) {
            string code = ht.huffmanCodes[word];
            writeBuffer(outFile, buffer, bitCount, code);
        }
    }

    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        outFile.write(reinterpret_cast<const char*>(&buffer), 1);
    }    
    //outFile.close();

    auto end = chrono::steady_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Tempo de compactacao de arquivo " << decompressedFilename << (byWord ? " por palavra: " : " por letra: ") << duration.count() << "ms\n";

}