

#include "common.h"

void hexToByte(string &hex, vector<unsigned char> &bytes)
{
	if (hex.length() % 2 != 0)
	{
		error("hexToByte: hex.length() mod 2 != 0");
		return;
	}

	bytes.clear();
	for (unsigned int i = 0; i < hex.length(); i += 2)
	{
		string byteString = hex.substr(i, 2);
		unsigned char byte = (unsigned char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}
}

string toHex(unsigned int num)
{
	ostringstream ss;
	ss << hex << uppercase << setfill('0') << setw(2) << num;
	return ss.str();
}

string toHex(unsigned char ch)
{
	ostringstream ss;
	ss << hex << uppercase << setfill('0') << setw(2) << (int) ch;
	return ss.str();
}

string toHex(vector<unsigned char> &v)
{
	ostringstream ss;
	ss << hex << uppercase << setfill('0');
	for_each(v.cbegin(), v.cend(), [&](int c) { ss << std::setw(2) << c; });
	return ss.str();
}

void error(string mes)
{
	cout << "[ERROR] " << mes.c_str() << endl;
	pause();
}

void warn(string mes)
{
	cout << "[WARN] " << mes.c_str() << endl;
}

void pause()
{
	system("pause");
}

void pause(bool print)
{
	if (print) pause();
	else
	{
		cin.clear();
		while (cin.get() != '\n');
	}
}

void readline(string &str)
{
	char c; while ((c = cin.get()) == '\n');
	cin.putback(c); getline(cin, str);
}

// Converts char for printable form.
string parseForPrint(unsigned char ch)
{
	if (ch == 9)
		return string("[\\t]");
	else if (ch == 10)
		return string("[\\n]");
	else if (ch == 13)
		return string("[\\r]");
	else if (ch == 32)
		return string("[space]");
	else if ((ch >= 32 && ch <= 126) || (ch >= 128 && ch != 173 && ch != 160 && ch != 152))
		return string(1, ch);
	else
		return string("[?]");
}

