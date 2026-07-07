#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp"

using namespace std;

typedef int Header;

struct FileNode{
    string token;
    int left;
    int right;
};

void compress(HuffmanTree ht);