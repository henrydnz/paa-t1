#pragma once

#include <cstdlib>
#include <iostream>
#include <chrono> 
#include <fstream> 

using namespace std;

void clearScreen();

void showMenu();

string timeElapsed(string filename, string action, chrono::_V2::steady_clock::time_point start, chrono::_V2::steady_clock::time_point end);

string fileSize(string filename, string type, streampos filePointer);