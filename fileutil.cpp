

#include "fileutil.h"

void readFileBinnary(string path, vector<unsigned char> &buffer)
{
    cout << "Reading file: " << path.c_str() << endl;

    ifstream input(path, ios::binary);
    copy(istreambuf_iterator<char>(input), istreambuf_iterator<char>(), back_inserter(buffer));
	input.close();
}

void writeFileBinnary(string path, vector<unsigned char> &buffer)
{
	if (buffer.size() == 0)
	{
		warn("Can't write file with empty content: " + path);
		return;
	} else
		cout << "Writing file: " << path.c_str() << endl;

	string outdir = "";//= "output\\";
	//_mkdir(outdir.c_str());
	ofstream output(outdir + path, ios::out | ios::binary);
	output.write((char *) &buffer[0], buffer.size() * sizeof(char));
	output.close();
}

bool exists(string name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

