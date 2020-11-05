#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

#define MSG_SUCCESS 10
#define MSG_ERROR	11

wstring directoryPath;

int signatureStartPosition = 0;

//char signature[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

//char signature[16];
vector<char> signature;
string signatureString = "";
int signatureLength = 0;

vector<wstring> paths;
vector<wstring> result;

bool IsNotDotName(std::wstring const& Filename)
{
	return ((Filename != L".") && (Filename != L".."));
}

bool IsDirectory(DWORD FileAttrs)
{
	return ((0 != (FileAttrs & FILE_ATTRIBUTE_DIRECTORY)) && (0 == (FileAttrs & FILE_ATTRIBUTE_REPARSE_POINT)));
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
				if (0 == (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
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


bool signIsFound(wstring path)
{
	string temp = "", file = "";
	bool result;
	char newSymb;

	ifstream in(path, ios::binary);

	while (1)
	{
		newSymb = in.get();
		if (in.eof()) break;
		file += newSymb;
	}

	if (file.find(signatureString) == std::string::npos) result = false;
	else result = true;

	in.close();
	return result;
}

int readSignature()
{
	bool unsuccessReading = true;
	string signFilePath = "";
	ifstream inSign;
	char newSymb;
	char signatureCounter = 0;
	int symbolCounter = 0;
	int startSignaturePointer = 0;
	
	cout << "Enter name of file in current directory or full path to the file to read signature." << endl;
	cout << "For example: C:\\files\\signature.txt" << endl;

newPathLabel:
	cout << "Path to signature file: ";

	cin >> signFilePath;
	cout << signFilePath << endl;

	inSign.open(signFilePath, ios::binary);

	while (!inSign.is_open())
	{
		cout << endl << "ERROR: Sorry, It's unable to open the file..." << endl;
		cout << "Please, try again: ";
		cin >> signFilePath;
		cout << signFilePath << endl;
		inSign.open(signFilePath, ios::binary);
	}

	cout << "Signature file was successfully opened!" << endl << endl;

	cout << "Enter start position in file to read the signature: ";
	cin >> startSignaturePointer;

	cout << endl << "Enter signature length: ";
	cin >> signatureLength;
	
	while (unsuccessReading)
	{
		signature.clear();
		while (1)
		{
			newSymb = inSign.get();
			if (inSign.eof()) break;

			if (symbolCounter >= startSignaturePointer)
			{
				if (signatureCounter < signatureLength)
				{
					signature.push_back(newSymb);
					signatureCounter++;
				}
				else break;
			}
			symbolCounter++;
		}

		inSign.close();

		if (signatureCounter < signatureLength)
		{
			signatureCounter = 0;
			symbolCounter = 0;

			cout << endl << "ERROR: Too short file to read the signature :(" << endl;

			if (startSignaturePointer != 0)
			{
				cout << "Please, try another values! " << endl << endl;

				cout << "Position: ";
				cin >> startSignaturePointer;

				cout << "Length: ";
				cin >> signatureLength;

				inSign.open(signFilePath, ios::binary);
			}
			else
			{
				cout << "Please, try another file." << endl;
				signFilePath = "";
				goto newPathLabel;
			}
		}
		else unsuccessReading = false;
	}

	cout << endl;
	return MSG_SUCCESS;
}

void printSignature()
{
	cout << endl << "Signature: ";
	for (char i = 0; i < signatureLength; i++)
	{
		cout << signature[i];
	}
	cout << endl;
}

void createSignatureString()
{
	for (int i = 0; i < signatureLength; i++) signatureString += signature[i];
}
int main(int argc, char* argv[])
{
	cout << endl << "================ Start Signature Search program ================" << endl << endl;

	if (readSignature() == MSG_SUCCESS)
	{
		cout << "Successful readiong of signature!" << endl;
		printSignature();
	}

	createSignatureString();
	
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
	
	for (vector<wstring>::iterator it = paths.begin(); it < paths.end(); it++)
	{
		if (signIsFound(*it))
		{
			result.push_back(*it);
		}
	}

	if (result.empty()) cout << endl << "There are NO files includes signature!" << endl;
	else
	{
		cout << endl << "Signature was found in:" << endl;
		printVector(result);
	}
	
	cout << endl << "================ Finish Signature Search program ===============" << endl << endl;

	system("pause");

	return 0;
}