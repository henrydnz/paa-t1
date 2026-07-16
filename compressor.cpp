#include "compressor.hpp"

Compressor::Compressor(string originalFilename, bool byWord) {
    this->originalFilename = originalFilename + ".txt";
    this->compressedFilename = originalFilename + "_binary.bin";
    this->outputFilename = originalFilename + "_output.txt";
    this->byWord = byWord;
}

static void writeSymbols(ofstream& compressedFile, unordered_map<string, int> symbols){
    for(Symbol symbol : symbols){
        string token = symbol.first;
        int frequency = symbol.second;

        compressedFile.write(token.c_str(), token.length() + 1);
        compressedFile.write(reinterpret_cast<char*>(&frequency), sizeof(int));
    }
}

static void writeBitBuffer(ofstream& compressedFile, char& buffer, int& bitCount, uint64_t code){
    for(int i=(code>>CODE_SIZE)-1;i>=0;i--){
        buffer=(buffer<<1)|((code>>i)&1);
        bitCount++;
        if(bitCount==BUFFER_SIZE) {
            compressedFile.write(reinterpret_cast<const char*>(&buffer), 1);
            buffer=0;
            bitCount=0;
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
            writeBitBuffer(compressedFile, buffer, bitCount, codes[string(1, c)]);
        }
    } else {
        string word = "";
        while (originalFile.get(c)) {
            if (isLetter(c)) word+=c;
            else{
                if(!word.empty()){
                    writeBitBuffer(compressedFile, buffer, bitCount, codes[word]);
                    word="";
                }
                writeBitBuffer(compressedFile, buffer, bitCount, codes[string(1,c)]);
            }

        }
        if (!word.empty()) {
            writeBitBuffer(compressedFile, buffer, bitCount, codes[word]);
        }
    }

    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        compressedFile.write(reinterpret_cast<const char*>(&buffer), 1);
    } 
}

int Compressor::compress(){
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

    auto start = chrono::steady_clock::now();

    HuffmanTree ht(originalFile, byWord);

    this->leafCount = ht.symbolFrequencyTable.size();

    this->tokenCount = 0;
    for(pair<string, int> sym : ht.symbolFrequencyTable)
        this->tokenCount += sym.second;

    compressedFile.write(reinterpret_cast<const char*>(&this->leafCount), sizeof(int));
    compressedFile.write(reinterpret_cast<const char*>(&this->tokenCount), sizeof(int));

    writeSymbols(compressedFile, ht.symbolFrequencyTable);
    writeCompressedMessage(originalFile, compressedFile, ht.huffmanCodes);
    
    auto end = chrono::steady_clock::now();
    cout << timeElapsed(originalFilename, "comprimido", start, end);

    cout << fileSize(compressedFilename, "output", compressedFile.tellp());
    
    originalFile.close();
    compressedFile.close();

    return 0;
}

void Compressor::getSymbols(ifstream& compressedFile){
    string symbol;
    int size, frequency;
    for(int i = 0; i < this->leafCount ; i++) {
        getline(compressedFile, symbol, '\0');

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
    
    auto start = chrono::steady_clock::now();

    compressedFile.read(reinterpret_cast<char*>(&this->leafCount), sizeof(int));
    compressedFile.read(reinterpret_cast<char*>(&this->tokenCount), sizeof(int));
    
    getSymbols(compressedFile);

    HuffmanTree ht(symbols);
    this->treeRoot = ht.root;

    writeDecompressedMessage(compressedFile, outputFile);

    auto end = chrono::steady_clock::now();
    cout << timeElapsed(compressedFilename, "descomprimido", start, end);
    
    cout << fileSize(outputFilename, "output", outputFile.tellp());
    
    compressedFile.close();
    outputFile.close();

    return 0;
}