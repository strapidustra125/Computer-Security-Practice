#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;


string inFile = "";
string outFile = "encrypted.txt";

int c = 0, s = 0;

//bool error = false;
bool flag = false;

int eng[18] = { 'A', 'B', 'C', 'E', 'H', 'K', 'O', 'P', 'M', 'X', 'T', 'e', 'y', 'o', 'p', 'a', 'x', 'c' };
int rus[18] = { 'А', 'В', 'С', 'Е', 'Н', 'К', 'О', 'Р', 'М', 'Х', 'Т', 'е', 'у', 'о', 'р', 'а', 'х', 'с' };

bool isRus(char c)
{
	bool result;

	for (int i = 0; i < 18; i++)
	{
		if (c == rus[i]) return true;
	}

	return false;
}

bool isEng(char c)
{
	bool result;

	for (int i = 0; i < 18; i++)
	{
		if (c == eng[i]) return true;
	}

	return false;
}

char changeToEngLetter(char c)
{
	int pos;

	for (int i = 0; i < 18; i++)
	{
		if (c == rus[i]) pos = i;
	}

	return eng[pos];
}

char changeToRusLetter(char c)
{
	int pos;

	for (int i = 0; i < 18; i++)
	{
		if (c == eng[i]) pos = i;
	}

	return rus[pos];
}

string revString(string s)
{
	string temp = "";
	for (int i = s.length() - 1; i >= 0; i--)
	{
		temp += s[i];
	}
	return temp;
}

vector<string> deleteEngLetters(vector<string> file)
{
	vector<string> newVec = file;
	for (vector<string>::iterator i = newVec.begin(); i < newVec.end(); i++)
	{
		for (int j = 0; j < (*i).length(); j++)
		{
			if (isEng((*i)[j])) (*i)[j] = changeToRusLetter((*i)[j]);
		}
	}

	return newVec;
}

vector<string> readFile(ifstream &in)
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
	int code = c,
		temp;

	if (code > 0)
	{
		while (code != 0)
		{
			temp = code % 2;
			code = code / 2;

			if (temp == 1) s += '1';
			else if (temp == 0) s += '0';
		}

		if (s.length() < 8)
		{
			for (int i = s.length(); i < 8; i++)
			{
				s += '0';
			}
		}
		s = revString(s);
	}
	else
	{
		code = 256 + code;

		while (code != 0)
		{
			temp = code % 2;
			code = code / 2;

			if (temp == 1) s += '1';
			else if (temp == 0) s += '0';
		}

		if (s.length() < 8)
		{
			for (int i = s.length(); i < 8; i++)
			{
				s += '0';
			}
		}
		s = revString(s);
	}

	return s;
}

vector<string> addCode(string code, vector<string> file)
{
	vector<string>::iterator it = file.begin();
	vector<string> newVec = file;
	string temp,
		allCodeBin = "";
	int count = 0, cc;

	for (int i = 0; i < code.length(); i++)
	{
		allCodeBin += charToBinString(code[i]);
	}

	//out << "Код: " << allCodeBin << endl;

	for (vector<string>::iterator i = newVec.begin(); i < newVec.end(); i++)
	{
		for (int j = 0; j < (*i).length(); j++)
		{
			cc = (*i)[j];
			if (count == allCodeBin.length())
			{
				return newVec;
			}
			else if (isRus((*i)[j]))
			{
				if (allCodeBin[count] == '1')
				{
					(*i)[j] = changeToEngLetter((*i)[j]);
					cc = (*i)[j];
					c++;
				}
				count++;
				s++;
			}
		}
	}

	return newVec;
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

string readCode(vector<string> file)
{
	string code = "",
		temp = "";

	for (vector<string>::iterator i = file.begin(); i < file.end(); i++)
	{
		for (int j = 0; j < (*i).length(); j++)
		{
			if (isEng((*i)[j]))
				temp += '1';
			else if (isRus((*i)[j]))
				temp += '0';
			if (temp.length() == 8)
			{
				code += binStrToChar(temp);
				temp = "";
			}
		}

	}

	return code;
}

int analisisInput(vector<string> file)
{
	int count = 0;

	for (vector<string>::iterator i = file.begin(); i < file.end(); i++)
	{
		for (int j = 0; j < (*i).length(); j++) if (isRus((*i)[j])) count++;
	}

	return count;
}

void clearCode(string& code)
{
	for (int i = code.length() - 1; i >= 0; i--)
	{
		if (code[i] != '\0')
			break;
		else code.pop_back();
	}
}

int main()
{
	cout << endl << "================ Start Steganography_3_encrypt program ================" << endl << endl;

	vector<string> file;
	string sIn,
		code = "";
	int count;

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
	file = deleteEngLetters(file);
	count = analisisInput(file);

	cout << "----------------------------------------" << endl;

	cout << "Space number: " << count << endl;
	cout << "Available symbols to encrypt: " << count / 8 << endl;

	cout << "----------------------------------------" << endl << endl;


	cout << "Enter " << count / 8 << " symbols or smaller message to encrypt." << endl;
	cout << "Message: ";


	while (!flag)
	{
		getline(cin, code);

		if (count / 8 > file.size())
		{
			cout << "File size is too small for this message..." << endl;
			cout << "Try again: ";
		}
		else flag = true;
	}

	cout << endl << "Entered message: " << "\'" << code << "\'" << endl << endl;



	file = addCode(code, file);

	ofstream out(outFile);

	for (vector<string>::iterator i = file.begin(); i < file.end(); i++) out << *i << endl;

	cout << endl << "Encrypted message was hidden in file \'" << outFile << "\'" << endl;

	cout << endl << "================ Finish Steganography_3_encrypt program ===============" << endl << endl;

	system("pause");


	return 0;
}

