

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <array>
#include <string>
#include <sstream>
#include <math.h>
#include <iterator>
#include <iomanip>
#include <functional>
#include "locale.h"

using namespace std;

void hexToByte(string &hexStr, vector<unsigned char> &bytes);
string toHex(vector<unsigned char> &v);
string toHex(unsigned int num);
string toHex(unsigned char ch);

void error(string mes);
void warn(string mes);
void pause();
void pause(bool print);
void readline(string &str);
string parseForPrint(unsigned char ch);

