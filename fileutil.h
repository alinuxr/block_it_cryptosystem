

#pragma once

#include "common.h"
#include <direct.h> 
#include <fstream>

void readFileBinnary(string path, vector<unsigned char> &buffer);
void writeFileBinnary(string path, vector<unsigned char> &buffer);
bool exists(string name);
