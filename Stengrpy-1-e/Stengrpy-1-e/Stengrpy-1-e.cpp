#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

string inFile = "";
string outFile = "encrypted.txt";

bool error = false;
bool flag = false;

string revString(string s)
{
	string temp = "";
	for (int i = s.length() - 1; i >= 0; i--)
	{
		temp += s[i];
	}
	return temp;
}

vector<string> deleteEndSpaces(vector<string> file)
{
	vector<string> newVec;
	for (vector<string>::iterator i = file.begin(); i < file.end(); i++)
	{
		string temp = "";
		for (int j = (*i).length() - 1; j >= 0; j--)
		{
			if ((*i)[j] != ' ')
			{
				for (int k = j; k >= 0; k--)
				{
					temp += (*i)[k];
				}
				j = 0;
			}
		}
		temp = revString(temp);
		newVec.push_back(temp);
	}
	return newVec;
}

vector<string> readFile(ifstream& in)
{
	vector<string> file;
	string s;
	while (!in.eof())
	{
		getline(in, s);
		file.push_back(s);
	}
	return file;
}

string charToBinString(char c)
{
	string s = "";
	int code,
		temp;

	if (c < 0) code = 256 + c;
	else code = c;

	while (code != 0)
	{
		temp = code % 2;
		code = code / 2;

		if (temp == 1) s += '1';
		else if (temp == 0) s += '0';
	}

	if (s.length() < 8)
	{
		for (int i = s.length(); i < 9; i++)
		{
			s += '0';
		}
	}
	s = revString(s);

	return s;
}

vector<string> addCode(string code, vector<string> file)
{
	vector<string>::iterator it = file.begin();
	string temp;
	
	for (int i = 0; i < code.length(); i++)
	{
		temp = charToBinString(code[i]);
		for (int j = 0; j < 8; j++)
		{
			if (temp[j] == '1') (*it) += ' ';
			it++;
		}
	}
	
	return file;
}

char binStrToChar(string s)
{
	int code = 0,
		temp = 0;

	for (int i = s.length() - 1, j = 0; i >= 0; i--, j++)
	{
		if (s[i] == '1') temp = 1;
		else if (s[i] == '0') temp = 0;

		code += temp * pow(2, j);
	}
	return (char)code;
}


int main()
{
	cout << endl << "================ Start Steganography_1_encrypt program ================" << endl << endl;

	vector<string> file;
	string sIn;
	string code = "";

	ifstream in;


	cout << "Enter name of file in current directory or full path to the file to read text." << endl;
	cout << "For example: C:\\files\\inputText.txt" << endl;

newPathLabel:
	cout << "Path to text file: ";

	getline(cin, inFile);

	in.open(inFile);

	while (!in.is_open())
	{
		cout << "ERROR: Sorry, It's unable to open the file..." << endl;
		cout << "Please, try again: ";
		getline(cin, inFile);
		in.open(inFile);
	}

	cout << endl << "Text file was successfully opened!" << endl << endl;

	file = readFile(in);

	cout << "----------------------------------------" << endl;

	cout << "String number: " << file.size() << endl;
	cout << "Available symbols to encrypt: " << file.size() / 8 << endl;

	cout << "----------------------------------------" << endl << endl;


	cout << "Enter " << file.size() / 8 << " symbols or smaller message to encrypt." << endl;
	cout << "Message: ";

	SetConsoleCP(1251);

	while (!flag)
	{
		getline(cin, code);

		

		if (code.length() * 8 > file.size())
		{
			cout << "File size is too small for this message..." << endl;
			cout << "Try again: ";
		}
		else flag = true;
	}
	
	cout << endl << "Entered message: " << "\'"; 

	SetConsoleCP(866);
	setlocale(LC_ALL, "Russian");

	cout << code << "\'" << endl << endl;

	//code.erase(0, 1);
	
	file = deleteEndSpaces(file);

	file = addCode(code, file);

	ofstream out(outFile);

	for (vector<string>::iterator i = file.begin(); i < file.end(); i++) out << *i << endl;

	cout << endl << "Encrypted message was hidden in file \'" << outFile << "\'" << endl;

	cout << endl << "================ Finish Steganography_1_encrypt program ===============" << endl << endl;

	system("pause");

	return 0;
}

