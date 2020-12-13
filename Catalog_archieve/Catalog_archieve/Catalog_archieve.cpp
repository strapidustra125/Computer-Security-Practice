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

wstring objectPath;
vector<wstring> filePaths, directoryPaths;
vector<wstring> noRootFiles, noRootDirectorys;

ofstream out, newFileOut;
ifstream in, inArch, inFile;

string result = "",			// Результирующая строка для записи в архив
	actionChoiceString,		// Выбор: архивация/разархивация
	outFile,
	archieve,				// Прочитанный архив
	check = "",				// Прочитанная из архива сигнатура для проверки формата
	archKey = "",
	keyString = "",
	readNewFile = "",
	keyLength = "",
	temp = "",				// Временная переменная (к-во файлов)
	newFilePath = "",		// Путь новго файла из архива
	newFileSymbols = "",	// Содержимое нового файла из архива
	newDirPath = "",		// Путь новой директории из архива
	del = "",			// Удалить каталог
	newFileSizeString = "",
	signature = "123()_AaBb123((",
	key;					// Пароль архива

int counter = 0,			// Итератор по прочитанному архиву
	keyLengthInt,
	fileNumber = 0,
	directoryNumber = 0,
	newFileSize = 0,
	actionChoice;			// Выбор: архивация/разархивация

char newArchSymb;			// Посимвольное чтение архива
char command[200];

void sysCreateDir(string fileNameString)
{
	for (int i = 0; i < 100; i++) command[0] = 0;
	string s1 = "mkdir \"" + fileNameString + "\"";
	for (int i = 0; i < s1.length(); i++) command[i] = s1[i];
	system(command);
}

void sysDeleteDir(string fileNameString)
{
	for (int i = 0; i < 100; i++) command[0] = 0;
	string s1 = "rd /s /q \"" + fileNameString + "\"";
	for (int i = 0; i < s1.length(); i++) command[i] = s1[i];
	system(command);
}

void sysDeleteFile(string fileNameString)
{
	for (int i = 0; i < 100; i++) command[i] = 0;
	string s1 = "del \"" + fileNameString + "\"";
	for (int i = 0; i < s1.length(); i++) command[i] = s1[i];
	system(command);
}

string getArchieveName(wstring fileNameString)
{
	string result;
	
	for (int i = 0; i < fileNameString.length(); i++) result += fileNameString[i];

	result += ".lol";

	return result;
}

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

string readFileBinary(wstring fileName) 
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

void EnumerateObjects(std::wstring const& Path)
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
					directoryPaths.push_back(FullPath);
				}

				if (0 == (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					filePaths.push_back(FullPath);
				}

				if (IsDirectory(FindData.dwFileAttributes))
				{
					EnumerateObjects(FullPath);
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


bool checkChoice(string s)
{
	if (s.length() > 1)
	{
		cout << endl << "ERROR: Incorrect symbol!" << endl;
		cout << "Try again: ";
		return false;
	}

	if (s[0] == '1')
	{
		actionChoice = 1;
		return true;
	}
	else if (s[0] == '2')
	{
		actionChoice = 2;
		return true;
	}
	else
	{
		cout << endl << "ERROR: Incorrect symbol!" << endl;
		cout << "Try again: ";
		return false;
	}
}

string defineDirectory(wstring pathToFile)
{
	bool isFullPath = false;
	string result = "";
	bool devideFlag = false;
	int devidePosition = 0;

	// Проверка полного пути и поиск последнего разделителя
	for (int i = pathToFile.length() - 2; i >= 0; i--)
	{
		//cout << pathToFile[i] << endl;
		if (!devideFlag)
		{
			if (pathToFile[i] == '\\')
			{
				devidePosition = i;
				devideFlag = true;
			}
		}

		if(pathToFile[i] == ':') isFullPath = true;
	}

	if (!isFullPath)
	{
		cout << endl << "Error: It's not a full path!" << endl << endl;
		cout << "Try again: ";
		return "";
	}

	for (int i = 0; i < devidePosition + 1; i++)
	{
		result += pathToFile[i];
	}

	return result;
}

vector<wstring> createFilesVector(vector<wstring> vec, string rootPath)
{
	vector<wstring> result;
	wstring temp;

	//for (vector<wstring>::iterator it = vec.begin(); it < vec.end(); it++)
	for (int i = 0; i < vec.size(); i++)
	{
		temp = L"";
		for (int j = rootPath.length(); j < vec[i].length(); j++) temp += vec[i][j];
		result.push_back(temp);
	}

	return result;
}

bool checkSignature(string s)
{
	if (signature.length() != s.length()) return false;

	for (int i = 0; i < signature.length(); i++)
	{
		if (signature[i] != s[i]) return false;
	}

	return true;
}

bool checkStrings(string key, string s)
{
	if (key.length() != s.length()) return false;

	for (int i = 0; i < key.length(); i++)
	{
		if (key[i] != s[i]) return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	cout << endl << "================ Start Signature Search program ================" << endl << endl;

	cout << "Choose action:" << endl;
	cout << "\t1 - add files to archieve" << endl << "\t2 - extract files from archieve" << endl << endl;
	cout << "Action: ";
	cin >> actionChoiceString;

	while (checkChoice(actionChoiceString) == false)
		cin >> actionChoiceString;

	switch (actionChoice)
	{
	case 1:	// Заархивировать

		cout << endl << "Enter FULL path to directory to create archieve." << endl;
		cout << "Object: ";

	Read:
		wcin >> objectPath;

		outFile = defineDirectory(objectPath);
		if (outFile == "") goto Read;

		cout << "Work directory: " << outFile << endl << endl;

		EnumerateObjects(objectPath);

		if (filePaths.empty())
		{
			cout << endl << "Error: Directory is empty or doesn't exist!" << endl << endl;
			cout << "Try again: ";
			goto Read;
		}
		else
		{
			// Архивация

			cout << endl << "The list of the file root pathes:" << endl;
			printVector(filePaths);

			cout << endl << "The list of the directory pathes:" << endl;
			printVector(directoryPaths);

			noRootFiles = createFilesVector(filePaths, outFile);
			noRootDirectorys = createFilesVector(directoryPaths, outFile);

			/*
			cout << endl << "The list of the file pathes (without root directory):" << endl;
			printVector(files);
			*/

			cout << "Enter key: ";
			cin >> key;

			result += signature + to_string(key.length()) + "::" + key;


			// Список директорий через "::"

			result += to_string(noRootDirectorys.size()) + "::";

			for (int i = 0; i < noRootDirectorys.size(); i++)
			{
				for (int j = 0; j < noRootDirectorys[i].length(); j++) result += noRootDirectorys[i][j];
				result += "::";
			}


			// Список файлов с содержимым через "::"

			result += to_string(noRootFiles.size()) + "::";

			// Для каждого файла архива
			for (int i = 0; i < noRootFiles.size(); i++)
			{
				// Пишем путь в архив
				for (int j = 0; j < noRootFiles[i].length(); j++) result += noRootFiles[i][j];
				
				newFileSymbols = readFileBinary(filePaths[i]);
				result += "::";

				// Число символов в файле
				result += to_string(newFileSymbols.length());
				result += "::";

				// Содержимое файла
				result += newFileSymbols;
				newFileSymbols = "";
				result += "::";
			}

			//result += "::";

			out.open(getArchieveName(objectPath), ios::binary);
			out << chryptString(result);
			//out << result;

			cout << "Archieve was successfuly created!" << endl;

			for (int i = 0; i < objectPath.length(); i++) del += objectPath[i];
			sysDeleteDir(del);
		}
		
		break;
	
	
	case 2:	// разархивировать

		cout << endl << "Enter FULL path to \"*.lol\" archieve." << endl;

		cout << "Archieve: ";

	ArPath:
		wcin >> objectPath;

		inArch.open(objectPath, ios::binary);

		if (!inArch.is_open())
		{
			cout << "ERROR: Unable to open the file!" << endl;
			cout << "Try again: ";
			goto ArPath;
		}
		else
		{
			outFile = defineDirectory(objectPath);
			if (outFile == "") goto ArPath;

			while (1)
			{
				newArchSymb = inArch.get();
				if (inArch.eof()) break;
				archieve += newArchSymb;
			}

			archieve = chryptString(archieve);
			
			for (int i = 0; i < signature.length() && i < archieve.length(); i++) check += archieve[i];

			// Проверка сигнатуры

			if (!checkSignature(check))
			{
				cout << "ERROR: Incorrect archieve format!" << endl;
				cout << "Try again: ";
				check = "";
				goto ArPath;
			}
			else
			{
				// Разархивирование

				cout << endl << "Archieve was successfuly read!" << endl;

				counter = signature.length(); 

				// Длина пароля

				while (archieve[counter] != ':')
				{
					keyLength += archieve[counter];

					counter++;
				}

				counter += 2;

				keyLengthInt = stoi(keyLength);

				// Проверка пароля

				for (int i = 0; i < keyLengthInt; i++, counter++)
				{
					keyString += archieve[counter];
				}
							
				cout << "Enter archieve key: ";
			Key:
				cin >> key;

				if (!checkStrings(keyString, key))
				{
					cout << "ERROR: Incorrect key!" << endl;
					cout << "Try again: ";
					goto Key;
				}

				// Успешный пароль

				cout << endl << "Success!" << endl;


				// Количество директорий

				while (archieve[counter] != ':')
				{
					temp += archieve[counter];

					counter++;
				}

				counter += 2;
				directoryNumber = stoi(temp);
				temp = "";

				// Создание директорий

				if (directoryNumber == 0)
				{
					for (int i = 0; i < objectPath.length(); i++)
					{
						if (objectPath[i] != '.') temp += objectPath[i];
						else break;
					}

					sysCreateDir(temp);
					temp = "";
				}

				while (directoryNumber > 0)
				{
					while (archieve[counter] != ':')
					{
						newDirPath += archieve[counter];
						counter++;
					}
					counter += 2;

					sysCreateDir(outFile + newDirPath);

					newDirPath = "";

					directoryNumber--;
				}

				// Количество файлов

				while (archieve[counter] != ':')
				{
					temp += archieve[counter];

					counter++;
				}

				counter += 2;
				fileNumber = stoi(temp);
				temp = "";

				while (fileNumber > 0)
				{
					// Путь нового файла
					while (archieve[counter] != ':')
					{
						newFilePath += archieve[counter];
						counter++;
					}
					counter += 2;

					// Размер файла
					while (archieve[counter] != ':')
					{
						newFileSizeString += archieve[counter];
						counter++;
					}
					counter += 2;

					newFileSize = stoi(newFileSizeString);
					

					// Символы файла
					while (newFileSize)
					{
						newFileSymbols += archieve[counter];
						counter++;

						newFileSize--;
					}
					counter += 2;

					newFileOut.open(outFile + newFilePath, ios::binary);
					newFileOut << newFileSymbols;
					newFileOut.close();

					newFilePath = "";
					newFileSymbols = "";
					newFileSizeString = "";

					fileNumber--;
				}
			}
		}

		inArch.close();


		for (int i = 0; i < objectPath.length(); i++) del += objectPath[i];
		sysDeleteFile(del);

		break;

	default:

		cout << endl << "????" << endl;

		break;
	}

	

	/*
	while (paths.empty())
	{
		cout << "ERROR: There are NO files in this directory!" << endl;
		cout << "Please, try again: ";
		wcin >> objectPath;
		EnumerateFileObjects(objectPath);
	}

	cout << endl << "The list of the file pathes:" << endl;
	printVector(paths);
	*/

	//system("mkdir C:\\Users\\nik1441\\Documents\\1");

	/*
	string temp1 = readFileBinary("D:\\1.lol");
	string res1 = chryptString(temp1);
	ofstream out1("D:\\2.jpg", ios::binary);
	out1 << res1;
	*/	
	
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