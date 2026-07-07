#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hpp"

using namespace std;

struct FileNode{
    string token;
    int left;
    int right;
};

void compress(string filename);