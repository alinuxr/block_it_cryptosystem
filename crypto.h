

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <bitset>
#include <iomanip>
#include <iostream>

using namespace std;

typedef unsigned char u_int1;
typedef unsigned short u_int2;
typedef unsigned int u_int4;

void getKey(u_int2 K[], u_int4 Teta);
void SP(u_int2& X, u_int2 K, u_int1 P[], u_int1 S[]);
void decrypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds);
void crypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds);
DWORD WINAPI halfWeakKey(void* args);
void findWeakKeys();
u_int4 changed(u_int1 a[], u_int1 b[], u_int4 key);

void print(u_int1 text[], u_int4 size, const char* str);
void printHex(u_int1 text[], u_int4 size, const char* str);

