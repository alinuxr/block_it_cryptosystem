

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

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 crypto.cpp (length: 4124 ch)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "common.h"
#include "crypto.h"

void getKey(u_int2 K[], u_int4 Teta)
{
	u_int2 key1 = Teta & 0xFFFF;
	u_int4 key2 = (Teta & 0xFFFF0000) >> 16;

	K[0] = key1;
	K[1] = key2;
	K[2] = key1;
	K[3] = key2;
}

void SP(u_int2 &X, u_int2 K, u_int1 P[], u_int1 S[])
{
	X = X ^ K;
	u_int1 tempX[2];
	memcpy(tempX, &X, 2);
	u_int1 mX[4];
	mX[2] = (X & 0xF000) >> 12;
	mX[3] = (X & 0x0F00) >> 8;
	mX[0] = (X & 0x00F0) >> 4;
	mX[1] = (X & 0x000F);
	X = 0;
	for (int i = 0; i < sizeof(mX); i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (mX[i] == S[j])
			{
				X = X | (S[(j + 1) % 16] << (12 - 4 * i));
				break;
			}
		}
	}

	X = ((0x00FF & X) << 8) + ((0xFF00 & X) >> 8);
	u_int2 XX = X;
	memcpy(&XX, &X, 2);
	for (int i = 0; i < 16; i++)
	{
		u_int4 value = (XX >> (15 - P[i]) & 1UL);
		if (value > 0)
			X |= 1UL << 15 - P[(i + 1) % 16];
		else
			X &= ~(1UL << 15 - P[(i + 1) % 16]);
	}
}

void crypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds)
{
	u_int2 K[4];
	getKey(K, Teta);
	u_int4 key_whitening = ((K[0] ^ K[1]) << 16) + (K[0] ^ K[1]);

	for (int j = 0; j < rounds; j++)
	{
		for (int i = 0; i < size; i += 4)
		{
			u_int2 X1 = 0;
			u_int2 X2 = 0;
			memcpy(&X2, &input[i + 2], 2);
			memcpy(&X1, &input[i], 2);
			u_int2 XX; // = X2;
			memcpy(&XX, &X2, 2);
			u_int1 P[16];
			for (int i = 0; i < 16; i++)
				P[i] = (13 * i + 1) % 16;

			u_int1 S[] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
			SP(XX, K[j], P, S);
			u_int4 ret = (((X1 ^ XX) << 16) + X2) ^ key_whitening;
			input[i + 0] = (ret & 0x000000FF);
			input[i + 1] = (ret & 0x0000FF00) >> 8;
			input[i + 2] = (ret & 0x00FF0000) >> 16;
			input[i + 3] = (ret & 0xFF000000) >> 24;
		}
	}
}

void decrypt(u_int1 input[], u_int4 size, u_int4 Teta, u_int1 rounds)
{
	u_int2 K[4];
	getKey(K, Teta);
	u_int4 key_whitening = ((K[0] ^ K[1]) << 16) + (K[0] ^ K[1]);
	for (int j = rounds - 1; j >= 0; j--)
	{
		for (int i = 0; i < size; i += 4)
		{
			u_int4* temp = (u_int4*)(&input[i]);
			u_int4 whitening = key_whitening ^ *temp;
			u_int2 X1 = 0;
			u_int2 X2 = 0;
			X1 = (whitening & 0xFFFF0000) >> 16;
			X2 = (whitening & 0x0000FFFF);
			u_int2 XX; // = X2;
			memcpy(&XX, &X2, 2);
			u_int1 P[16];
			for (int i = 0; i < 16; i++)
				P[i] = (13 * i + 1) % 16;

			u_int1 S[] = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
			SP(XX, K[j], P, S);
			X1 = X1 ^ XX;
			input[i + 3] = (X2 & 0xFF00) >> 8;
			input[i + 2] = (X2 & 0x00FF);
			input[i + 1] = (X1 & 0xFF00) >> 8;
			input[i + 0] = (X1 & 0x00FF);
		}
	}
}

DWORD WINAPI halfWeakKey(void* args)
{
	u_int4 in = 1243467123;
	u_int1 input[4];
	u_int4 out;
	int i = (int)args;
	printf("Start thread #%d\n", i);
	for (u_int4 Teta = i * 1000000000; Teta < (i + 1) * 1000000000; Teta++)
	{
		memcpy(&input, &in, 4);
		crypt(input, 4, Teta, 4);
		crypt(input, 4, Teta, 4);
		memcpy(&out, &input, 4);
		if ((Teta - i * 1000000000) % 100000000 == 0)
		{
			printf("[%d] -> % u\n", i, Teta);
		}

		if (in == out)
		{
			printf("Find! % u\n", Teta);
			system("pause");
		}
	}

	return NULL;
}

void findWeakKeys()
{
	const int t_count = 4;
	HANDLE tids[t_count];
	time_t timer = time(NULL);
	for (int i = 0; i < t_count; i++)
	{
		tids[i] = CreateThread(0, 0, halfWeakKey, (void*)i, 0, NULL);
		if (0 == tids[i])
		{
			printf("pthread_create failed.errno: % d\n", errno);
			return;
		}
	}

	WaitForMultipleObjects(t_count, tids, TRUE, INFINITE);
	for (int i = 0; i < t_count; i++)
		CloseHandle(tids[i]);

	printf("Time %ld \n", (int)(time(NULL) - timer));
	system("pause");
}

u_int4 changed(u_int1 a[], u_int1 b[], u_int4 key)
{
	u_int4 count = 0;
	for (int i = 0; i < 4; i++)
	{
		a[i] = a[i] ^ b[i];
		for (u_int4 j = 0, k = 1; j < sizeof(a[i]) * 8; j++)
		{
			if ((a[i] & k) > 0)
				count++;
			k = k << 1;
		}
	}
	return count;
}

void print(u_int1 text[], u_int4 size, const char* str)
{
	printf("%s: ", str);
	for (int i = 0; i < size; i++)
		printf("%c", text[i]);
	printf("\n");
}

void printHex(u_int1 text[], u_int4 size, const char* str)
{
	printf("%s: ", str);
	for (int i = 0; i < size; i++)
		printf("%02hX ", text[i]);
	printf("\n");
}
