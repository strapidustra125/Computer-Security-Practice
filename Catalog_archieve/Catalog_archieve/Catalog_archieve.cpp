#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

using namespace std;

#define MSG_SUCCESS 10
#define MSG_ERROR	11

#define POLINOM		0b10010011

wstring directoryPath;
vector<wstring> paths;

bool IsNotDotName(wstring const& Filename)
{
	return ((Filename != L".") && (Filename != L".."));
}

bool IsDirectory(DWORD FileAttrs)
{
	return ((0 != (FileAttrs & FILE_ATTRIBUTE_DIRECTORY)) && (0 == (FileAttrs & FILE_ATTRIBUTE_REPARSE_POINT)));
}

string readFile(const wstring& fileName) 
{
	ifstream f(fileName);
	stringstream s;
	s << f.rdbuf();
	return s.str();
}

string readFileBinary1(const std::wstring& fileName) 
{
	std::ifstream f(fileName, ios::binary);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}

string readFileBinary(string fileName) 
{
	string result = "";
	char newSymb;
	ifstream file(fileName, ios::binary);

	while (1)
	{
		newSymb = file.get();
		if (file.eof()) 
			break;
		result += newSymb;
	}

	file.close();
	return result;
}

void EnumerateFileObjects(std::wstring const& Path)
{
	using namespace std;

	WIN32_FIND_DATAW FindData;
	wstring const Mask = Path + L"\\*";
	HANDLE  const hFind = FindFirstFileW(Mask.c_str(), &FindData);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			if (IsNotDotName(&FindData.cFileName[0]))
			{
				wstring const FullPath = Path + L"\\" + &FindData.cFileName[0];

				//if (FindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				if (0 != (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					paths.push_back(FullPath);
				}

				if (IsDirectory(FindData.dwFileAttributes))
				{
					EnumerateFileObjects(FullPath);
				}
			}
		} while (NULL != FindNextFileW(hFind, &FindData));

		FindClose(hFind);
	}
}

void printVector(vector<wstring> vec)
{
	for (vector<wstring>::iterator it = vec.begin(); it < vec.end(); it++)
	{
		wcout << '\t' << *it << endl;
	}
}

void createCatalogTree(vector<wstring> vec)
{
	wstring mkDir = L"mkdir ";
	for (vector<wstring>::iterator it = vec.begin(); it < vec.end(); it++)
	{
		//system(*(it));
	}

}

string chryptString(string str)
{
	string result = "";

	for (int i = 0; i < str.length(); i++)
	{
		result += (int)str[i] ^ POLINOM;
	}
	
	return result;
}


int main(int argc, char* argv[])
{
	cout << endl << "================ Start Signature Search program ================" << endl << endl;

	/*
	cout << endl << "Enter path to directory for Signature Search." << endl;
	cout << "Directory: ";
	wcin >> directoryPath;

	EnumerateFileObjects(directoryPath);

	while (paths.empty())
	{
		cout << "ERROR: There are NO files in this directory!" << endl;
		cout << "Please, try again: ";
		wcin >> directoryPath;
		EnumerateFileObjects(directoryPath);
	}

	cout << endl << "The list of the file pathes:" << endl;
	printVector(paths);
	*/

	//system("mkdir C:\\Users\\nik1441\\Documents\\1");

	string temp1 = readFileBinary("D:\\144166.lol");
	string res1 = chryptString(temp1);
	ofstream out1("D:\\new.jpg", ios::binary);
	out1 << res1;

	/*
	string temp2 = readFileBinary("D:\\144166.lol");
	string res2 = chryptString(temp2);
	ofstream out2("D:\\144166.lol", ios::binary);
	out2 << res2;
	*/
	

	cout << endl << "================ Finish Signature Search program ===============" << endl << endl;

	system("pause");

	return 0;
}