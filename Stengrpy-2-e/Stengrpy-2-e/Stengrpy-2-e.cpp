#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

string inFile = "";
string outFile = "encrypted.txt";

//bool error = false;
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

vector<string> deleteSpaces(vector<string> file)
{
	vector<string> newVec = file;
	int l, k, count = 0;
	for (vector<string>::iterator i = newVec.begin(); i < newVec.end(); i++)
	{
		l = (*i).length();
		for (int j = 0; j < l; j++)
		{
			if ((*i)[j] == ' ')
			{
				while ((*i)[j + 1] == ' ') (*i).erase(j + 1, 1);
				l = (*i).length();
			}
		}
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
	string temp = "",
		allCodeBin = "";
	int count = 0;

	for (int i = 0; i < code.length(); i++)
	{
		allCodeBin += charToBinString(code[i]);
	}

	for (vector<string>::iterator i = newVec.begin(); i < newVec.end(); i++)
	{
		for (int j = 0; j < (*i).length(); j++)
		{
			if (count < allCodeBin.length())
			{
				temp += (*i)[j];
				if ((*i)[j] == ' ')
				{
					if (allCodeBin[count] == '1') temp += ' ';
					count++;
				}
			}
			else temp += (*i)[j];
		}
		(*i) = temp;
		temp = "";
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
			if ((*i)[j] == ' ' && j != (*i).length() - 1)
			{
				if ((*i)[j + 1] == ' ')
				{
					temp += '1';
					j++;
				}
				else temp += '0';
			}
			else if ((*i)[j] == ' ' && j == (*i).length() - 1) temp += '0';

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
		for (int j = 0; j < (*i).length(); j++)
		{
			if ((*i)[j] == ' ') count++;
		}
	}

	return count;
}

int main()
{
	cout << endl << "================ Start Steganography_2_encrypt program ================" << endl << endl;

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
	file = deleteSpaces(file);
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

	cout << endl << "================ Finish Steganography_2_encrypt program ===============" << endl << endl;

	system("pause");






	/*
	in >> sIn;
	if (sIn == "write")
	{
		getline(in, code);
		code.erase(0, 1);
		file = readFile();
		file = deleteSpaces(file);
		count = analisisInput(file);
		if (count / 8 < code.length())
		{
			out << "Шифруемый код не умещается в сообщении..." << endl;
			return 0;
		}
		file = addCode(code, file);

		for (vector<string>::iterator i = file.begin(); i < file.end(); i++) out << *i << endl;
	}
	else if (sIn == "read")
	{
		getline(in, code);
		file = readFile();
		out << "Зашифрованное сообщение: " << readCode(file) << endl;
	}
	else if (sIn == "analysis")
	{
		getline(in, code);
		file = readFile();
		file = deleteSpaces(file);
		count = analisisInput(file);

		out << "Для шифрования доступно " << count << " бит, т.е. " << count / 8 << " букв..." << endl;
	}

	//out << (int)'A' << "\t\t" << charToBinString('A') << endl;
	//out << (int)'а' << "\t\t" << charToBinString('а') << "\t\t" << binStrToChar(charToBinString('а')) << endl;

	//out << "Всего: " << s << endl;
	//out << "Заменено: " << c << endl;

	file = readFile();
	file = deleteSpaces(file);

	for (vector<string>::iterator i = file.begin(); i < file.end(); i++) out << *i << endl;


	*/




	return 0;
}

