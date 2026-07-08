#include "arquivoManager.hpp"
#include "huffman.hpp"

static HuffmanTree getTree(ifstream& inFile, int leafCount){
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

    return HuffmanTree(symbols);
}

void Compressor::decompress(){
    ifstream compressed(compressedFilename, ios::binary);
    int leafCount, tokenCount;

    compressed.read(reinterpret_cast<char*>(&leafCount), sizeof(int));
    compressed.read(reinterpret_cast<char*>(&tokenCount), sizeof(int));
    
    HuffmanTree ht = getTree(compressed, leafCount);

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
        
        if(buffer & 0x80) {
            currentNode = currentNode->right;
        }

        else {
            currentNode = currentNode->left;
        }
        buffer = buffer << 1; bitCount--;

        if(currentNode->isLeaf()) {
            decompressed << currentNode->token;
            currentNode = ht.root;
            foundTokens++;
        }
    }
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
    ifstream inFile(decompressedFilename);
    HuffmanTree ht(inFile, byWord);

    ht.showTree();// DEBUG
    
    ofstream outFile(compressedFilename, ios::binary);

    outFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    outFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));

    for(pair<string, int> symbol : ht.symbols)  
        writeSymbol(outFile, symbol);

    inFile.clear();
    inFile.seekg(0, ios::beg);

    char buffer = 0;
    int bitCount = 0;
    if (!byWord) {
        char c;
        while (inFile.get(c)) {
            string code = ht.huffmanCodes[string(1, c)];
            writeBuffer(outFile, buffer, bitCount, code);
            
        }
    } else {
        string word;
        while (inFile >> word) {
            string code = ht.huffmanCodes[word];
            writeBuffer(outFile, buffer, bitCount, code);
        }
    }

    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        outFile.write(reinterpret_cast<const char*>(&buffer), 1);
    }    
    //outFile.close();
}