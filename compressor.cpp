#include "compressor.hpp"

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

void Compressor::writeCompressedMessage(ifstream& originalFile, ofstream& compressedFile, HuffmanCodes codes){
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


int Compressor::compress(){
    auto start = chrono::steady_clock::now();

    ifstream originalFile(originalFilename);
    if(!originalFile.is_open()){
        cerr << "Erro ao abrir o arquivo original " << originalFilename 
            << ".\nCertifique-se de que o arquivo esta localizado na pasta raiz\ne que o nome esta correto.\n";
        return 1;
    }

    ofstream compressedFile(compressedFilename, ios::binary);
    if(!compressedFile.is_open()){
        cerr << "Erro ao criar o arquivo " << compressedFilename << " para a compressao\n";
        return 1;
    }

    HuffmanTree ht(originalFile, byWord);
    
    compressedFile.write(reinterpret_cast<char*>(&ht.leafCount), sizeof(int));
    compressedFile.write(reinterpret_cast<char*>(&ht.tokenCount), sizeof(int));

    writeSymbols(compressedFile, ht.symbols);
    writeCompressedMessage(originalFile, compressedFile, ht.huffmanCodes);
    
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    int time_elapsed = duration.count();
    
    cout << "Arquivo " << originalFilename << " foi comprimido com sucesso! (Tempo:" << time_elapsed << "ms)\n";
    cout << "Output: " << compressedFilename << " (Tamanho: " << static_cast<double>(compressedFile.tellp())/1024.0 << "kb)" << endl << endl;
    
    originalFile.close();
    compressedFile.close();

    return 0;
}

void Compressor::getSymbols(ifstream& compressedFile){
    string symbol;
    int size, frequency;
    for(int i = 0; i < this->leafCount ; i++) {
        compressedFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        symbol.resize(size);

        compressedFile.read((&symbol[0]), size);

        compressedFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));

        symbols.push_back(make_pair(symbol, frequency));
    }
}

void Compressor::writeDecompressedMessage(ifstream& compressedFile, ofstream& outputFile){
    char buffer;
    int bitCount = 0;
    int foundTokens = 0;
    Node* currentNode = treeRoot;

    while(foundTokens < this->tokenCount) {
        if(bitCount == 0) {
            buffer = compressedFile.get();
            if (compressedFile.eof()) {cerr << "EOF!" << endl; break;}
            bitCount = 8;
        }

        currentNode = (buffer & 0x80) ? currentNode->right : currentNode->left;

        buffer = buffer << 1; 
        bitCount--;

        if(currentNode->isLeaf()) {
            outputFile << currentNode->token;
            currentNode = treeRoot;
            foundTokens++;
        }
    }
}

int Compressor::decompress(){
    auto start = chrono::steady_clock::now();

    ifstream compressedFile(compressedFilename, ios::binary);
    if(!compressedFile.is_open()){
        cerr << "Erro ao abrir o arquivo comprimido " << compressedFilename
            << ".\nCertifique-se de que o arquivo esta localizado na pasta raiz\ne que o nome esta correto.\n";
        return 1;
    }

    ofstream outputFile(outputFilename);
    if(!outputFile.is_open()){
        cerr << "Erro ao criar o arquivo de saida " << outputFilename << ".\n";
        return 1;
    }

    compressedFile.read(reinterpret_cast<char*>(&this->leafCount), sizeof(int));
    compressedFile.read(reinterpret_cast<char*>(&this->tokenCount), sizeof(int));
    
    getSymbols(compressedFile);

    HuffmanTree ht(symbols);
    this->treeRoot = ht.root;

    writeDecompressedMessage(compressedFile, outputFile);

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    int time_elapsed = duration.count();

    cout << "Arquivo " << compressedFilename << " foi descomprimido com sucesso! (Tempo:" << time_elapsed << "ms)\n";
    cout << "Output: " << outputFilename << " (Tamanho: " << static_cast<double>(outputFile.tellp())/1024.0 << "kb)" << endl << endl;
    compressedFile.close();
    outputFile.close();

    return 0;
}