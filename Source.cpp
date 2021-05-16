#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Date
{
	int Day, Month, Year;
};

struct Human
{
	string FIO;
	Date DateOfBirth;
	long long PassportNumber;

	void Print()
	{
		cout << endl;
		cout << "ФИО: " << FIO << endl;
		cout << "Дата рождения: " << DateOfBirth.Day << "." << DateOfBirth.Month << "." << DateOfBirth.Year << endl;
		cout << "Номер паспорта: " << PassportNumber << endl;
	}

	void GenerateData()
	{
		string names[] = { "Иван", "Юрий", "Алексей", "Артём", "Дмитрий" };
		string surnames[] = { "Иванов" , "Ананин", "Тарасов" };
		string patronymic[] = { "Андреевич", "Вадимович", "Михаилович", "Кирилович", "Ильич" };

		FIO = surnames[rand() % 3] + " " + names[rand() % 5] + " " + patronymic[rand() % 5];
		DateOfBirth.Day = 1 + rand() % 28;
		DateOfBirth.Month = 1 + rand() % 12;
		DateOfBirth.Year = 1900 + rand() % 200;
		PassportNumber = 5700000000 + (rand() % 100) * 1000000 + rand() % 1000000;
	}
};

vector<Human> CreateArray(int size = 100)
{
	vector<Human> res;
	for (int i = 0; i < size; i++)
	{
		Human newEl;
		newEl.GenerateData();
		res.push_back(newEl);
	}
	return res;
}

void PrintArray(vector<Human> humans)
{
	if (humans.size() < 1)
	{
		cout << "Список пуст" << endl;
		return;
	}
	for (int i = 0; i < humans.size(); i++)
	{
		cout << "Элемент " << i + 1 << endl;
		humans[i].Print();
	}
}

bool KMP(vector<Human> humans, Human key)
{
    string listOfNumbers;

    string substring = "";
    substring += to_string(key.PassportNumber);

    for (int i = 0; i < humans.size(); i++)
    {
        string newLine = "";
        newLine += to_string(humans[i].PassportNumber);
        listOfNumbers += newLine;
    }

    bool result = false;
    // Создание пи-таблицы
    vector<int> pi(substring.size());
    for (int j = 0, i = 1; i < substring.size(); i++) {
        while ((j > 0) && (substring[i] != substring[j]))
            j = pi[j - 1];
        if (substring[i] == substring[j])
        {
            j++;
        }
        pi[i] = j;
    }

    // Поиск
    for (int j = 0, i = 0; i < listOfNumbers.size(); ++i) {
        while ((j > 0) && (substring[j] != listOfNumbers[i]))
            j = pi[j - 1];
        j++;
        if (j == substring.size())
            result = true;
    }

    return result;
}
struct Cell
{
    char Symbol;
    int Bias;

    Cell(char c, int b)
    {
        Symbol = c;
        Bias = b;
    }
};

bool BM(vector<Human> humans, Human key)
{
    string listOfNumbers;

    string substring = "";
    substring += to_string(key.PassportNumber);

    for (int i = 0; i < humans.size(); i++)
    {
        string newLine = "";
        newLine += to_string(humans[i].PassportNumber);
        listOfNumbers += newLine;
    }

    //cout << substring << endl;
    //cout << listOfNumbers << endl;

    // Создание таблицы
    vector<Cell> table;
    for (int i = substring.size() - 2; i >= 0; i--)
    {
        // Поиск равных символов в таблице 
        bool isEqual = false;
        for (int j = 0; j < table.size() && !isEqual; j++)
        {
            isEqual = table[j].Symbol == substring[i];
        }
        if (!isEqual)
        {
            table.push_back(Cell(substring[i], substring.size() - i - 1));
        }
    }

    // Добавление последнего элемента
    reverse(table.begin(), table.end());
    bool isEqual = false;
    for (int j = 0; j < table.size() && !isEqual; j++)
    {
        isEqual = table[j].Symbol == substring[substring.size() - 1];
    }
    if (!isEqual)
    {
        table.push_back(Cell(substring[0], substring.size()));
    }

    // Поиск
    bool isElementFind = false;
    for (int i = substring.size() - 1; !isElementFind && i < listOfNumbers.size(); )
    {
        // сравнение символов
        int j = substring.size() - 1;
        while (j >= 0 && listOfNumbers[i - substring.size() + 1 + j] == substring[j]) j--;

        if (j < 0)
        {
            isElementFind = true;
        }
        else
        {
            // поиск символа в таблице
            int k = 0;
            while (k < table.size() && table[k].Symbol != listOfNumbers[i]) k++;
            if (k == table.size())
            {
                i += substring.size();
            }
            else
            {
                i += table[k].Bias;
            }
        }
    }
    return isElementFind;
}


int main()
{
    system("color F0");
	setlocale(LC_ALL, "rus");
	vector<Human> humans = CreateArray(100);
    PrintArray(humans);

    Human key;
    cout << endl << "Введите номер паспорта, по которому необходимо искать элемент: ";
    cin >> key.PassportNumber;

    cout << "Бойера-Мура" << endl;
    if (BM(humans, key))
    {
        cout << "Элемент найден" << endl;
    }
    else
    {
        cout << "Элемент не найден" << endl;
    }

    cout << "КМП" << endl;
    if (KMP(humans, key))
    {
        cout << "Элемент найден" << endl;
    }
    else
    {
        cout << "Элемент не найден" << endl;
    }

    return 0;
}
	