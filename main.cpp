
#define _CRT_SECURE_NO_WARNINGS

#include "common.h"
#include "fileutil.h"
#include "crypto.h"

int main()
{
	printf("input key: ");
	u_int4 key;
	scanf("%d", &key);

	std::bitset<32> kb(key);
	cout << "key (bin): 0b" << kb << endl;

	printf(" - - -\n");

	vector<unsigned char> raw;
	readFileBinnary("../input.txt", raw);

	int size = raw.size();
	int pad = (4 - (size % 4)) % 4;
	if (pad > 0)
		printf("Text will be padded with %d zeros\n", pad);

	int rawsize = size + pad + 4;
	u_int1 *data = (u_int1*) malloc(rawsize);
	memset(data, 0, rawsize);
	data[rawsize - 1] = pad;

	for (int i = 0; i < raw.size(); i++)
		data[i] = raw[i];

	printHex(data, size, "source text (hex)\t");
	print(data, size, "source text (plain)\t");

	printf(" - - -\n");

	u_int1* data2 = (u_int1*)malloc(rawsize);
	memcpy(data2, data, rawsize);

	key = 0b11111111000000001111111100000000;

	crypt(data, rawsize, key, 4);
	printHex(data, rawsize, "encrypted (hex) \t");

	key = 0b11111111000000001111111100000000;

	crypt(data, rawsize, key, 4);
	print(data, rawsize, "encrypted 2 (plain) \t");

	raw.clear();
	for (int i = 0; i < rawsize; i++)
		raw.push_back(data[i]);
	writeFileBinnary("../encrypted.txt", raw);

	//decrypt(data, rawsize, key, 4);
	//printf(" - - -\n");
	//printHex(data, size, "decrypted (hex) \t");
	//print(data, size, "decrypted (plain)\t");
	
	//findWeakKeys();

	/*printf("\n");
	u_int4 in = 23423412347;
	u_int1 input[4];
	memcpy(&input, &in, 4);
	u_int4 t_crypted;
	for (int i = 0; i < sizeof(input) * 8; i++)
	{
		printf("%d:\n", i);
		for (int j = 0; j < 4; j++)
		{
			u_int1 crypted_clear[4];
			memcpy(&crypted_clear, &in, 4);
			crypt(crypted_clear, 234235672, j);
			t_crypted = in ^ (1 << i);
			u_int1 crypted_error[4];
			memcpy(&crypted_error, &t_crypted, 4);
			crypt(crypted_error, 234235672, j);
			u_int4 round_one = changed(crypted_clear, crypted_error, 234235672);
			printf("\t%d -> % u\n", j, round_one);
		}
		printf("\n");
	}*/

	printf("\n");
	system("pause");
	return 0;
}
