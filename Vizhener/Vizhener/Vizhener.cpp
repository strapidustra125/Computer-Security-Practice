#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

#define ALPHA_LENGTH 74

char alphabet[ALPHA_LENGTH] = 
{ 
    'а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ',
    'ы', 'M', 'э', 'ю', 'я',
    'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ',
    'Ы', 'Ь', 'Э', 'Ю', 'Я',
    ' ', '-', '!', '.', '(', ')', '?', ','
};

vector<int> keyIntVec, messageIntVec, resultMesIntVec;
string key = "", message = "", resultMes = "";




int findCharCode(char c)
{
    for (int i = 0; i < ALPHA_LENGTH; i++)
    {
        if (c == alphabet[i]) return i;
    }

    return -1;
}

int fillIntVector(string str, vector<int>* vec)
{
    vector<int> result;
    int newCode;
    for (int i = 0; i < str.length(); i++)
    {
        newCode = findCharCode(str[i]);
        if (newCode < 0) return -1;
        else vec->push_back(newCode);
    }

    return 1;
}




string enchrypt(string message, string key)
{
    string result = "";

    if (fillIntVector(message, &messageIntVec) < 0) cout << "ERROR: Unknown symbol" << endl;

    if (fillIntVector(key, &keyIntVec) < 0) cout << "ERROR: Unknown symbol" << endl;

    vector<int>::iterator m = messageIntVec.begin();
    int k = 0;

    for ( ; m < messageIntVec.end(); m++, k = (k+1) % keyIntVec.size())
    {
        result += alphabet[(*m + keyIntVec[k]) % ALPHA_LENGTH];
    }

    return result;
}

string dechrypt(string message, string key)
{
    string result = "";

    if (fillIntVector(message, &messageIntVec) < 0) cout << "ERROR: Unknown symbol" << endl;
    
    if (fillIntVector(key, &keyIntVec) < 0) cout << "ERROR: Unknown symbol" << endl;

    vector<int>::iterator m = messageIntVec.begin();
    int k = 0;

    for (; m < messageIntVec.end(); m++, k = (k + 1) % keyIntVec.size())
    {
        if (*m < keyIntVec[k]) result += alphabet[(ALPHA_LENGTH - (keyIntVec[k] - *m)) % ALPHA_LENGTH];
        else result += alphabet[(*m - keyIntVec[k]) % ALPHA_LENGTH];
    }

    return result;
}


bool checkString(string str)
{
    bool result;

    for (int i = 0; i < str.length(); i++)
    {
        result = false;

        for (int j = 0; j < ALPHA_LENGTH; j++)
        {
            if (str[i] == alphabet[j]) result = true;
        }

        if (result == false)
        {
            cout << endl << "ERROR: Symbol \'" << str[i] << "\' in string \"" << str << "\" is unacceptable." << endl;
            cout << "Position: " << i + 1 << endl << endl;
            cout << "Try again: ";

            return result;
        }   
    }

    return true;
}

bool checkChoice(int n)
{
    if (n == 1) return true;
    else if (n == 2) return true;
    else
    {
        cout << endl << "ERROR: Incorrect symbol!" << endl;
        cout << "Try again: ";
        return false;
    }
}


int main()
{
    string result = "";
    int actionChoice;

    cout << endl << "================ Start Vizhener's cipher program ================" << endl << endl;

    SetConsoleCP(1251);

    cout << "Enter message to translate. Russian symbols only!" << endl;
    cout << "Message: ";
    getline(cin, message);

    while (checkString(message) == false) getline(cin, message);

    cout << "Enter key to enchrypt message. Russian symbols only!" << endl;
    cout << "Key: ";
    getline(cin, key);

    while (checkString(key) == false) getline(cin, key);

    cout << endl;

    cout << "Choose action:" << endl;
    cout << "\t1 - enchrypt" << endl << "\t2 - dechrypt" << endl << endl;
    cout << "Action: ";
    cin >> actionChoice;

    while (checkChoice(actionChoice) == false) cin >> actionChoice;

    switch (actionChoice)
    {
    case 1:
        result = enchrypt(message, key);
        break;
    case 2:
        result = dechrypt(message, key);
        break;
    }
    
    SetConsoleCP(866);
    setlocale(LC_ALL, "Russian");

    cout << endl << "Translated message: " << result << endl << endl;

    cout << endl << "================ Finish Vizhener's cipher program ===============" << endl << endl;

    system("pause");

    return 0;
}
