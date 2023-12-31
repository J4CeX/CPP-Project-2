#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<chrono>
#include<vector>
#include<cmath>
#include<iomanip>

using namespace std;

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::system_clock> Clock_val;

struct data
{
	int id;
	int size;
	vector<int> numbers;
};

struct sort
{
	int sortId;
	int id;
	int size;
	string type;
	double time;
};

void mergeSort(int *,int, int, int *);
void merge(int *, int, int, int, int *);
void quickSort(int *, int, int);
void bucketSort(int *, int);
void bubbleSort(int *, int);
void insertSort(int *, int);
void setData(vector<data> &);
void setSort(vector<sort> &);
void show(vector<data> &, vector<sort> &);
void showData(vector<data> &, int);
void showSort(vector<sort> &, int, int);
void generateData(int n, int maxValue = 0, int nSize = 0);
void generateSort();
int convertStringToInt(string);
void addData(vector<data> &);
void sortData(vector<data> &, vector<sort> &);
void showRankingList(vector<sort> &);
void quickSortRankingList(double *, int, int);
void error();

int main()
{
	vector<data> dataBase;
	vector<sort> sortBase;

	int option;
	while(true)
	{
		setData(dataBase);
		setSort(sortBase);

		system("CLS");
		cout<<"=============================="<<endl
			<<"Witaj w kalkulatorze sortowan!"<<endl
			<<"=============================="<<endl
			<<"Opcje:"<<endl
			<<"1. Wyswietl zestaw"<<endl
			<<"2. Dodaj zestaw"<<endl
			<<"3. Sortuj zestaw"<<endl
			<<"4. Wyswietl ranking sortowan"<<endl
			<<"0. Zakoncz"<<endl
			<<"-> "; cin>>option;
		switch(option)
		{
			case 0:
				system("CLS");
				cout<<"======="<<endl
					<<"Koniec!"<<endl
					<<"=======";
				return 0;
			case 1:
				show(dataBase, sortBase);
				break;
			case 2:
				addData(dataBase);
				break;
			case 3:
				sortData(dataBase, sortBase);
				break;
			case 4:
				showRankingList(sortBase);
				break;
			default:
				error();
				continue;
		}
	}

	return 0;
}

void setData(vector<data> &dataBase)
{
	dataBase.clear();
	ifstream file;
	file.open("data.txt");

	if(!file.good())
	{
		file.close();

		system("CLS");
		cout<<"============"<<endl
			<<"Brak danych!"<<endl
			<<"============"<<endl;
		_sleep(2000);

		system("CLS");
		cout<<"================"<<endl
			<<"Generuje dane..."<<endl
			<<"================"<<endl;

		generateData(5);
		_sleep(2000);

		system("CLS");
		cout<<"============================"<<endl
			<<"Pomyslnie wygenerowano dane"<<endl
			<<"============================"<<endl;
		_sleep(2000);

		file.open("data.txt");
	}

	string line;

	while(!file.eof())
	{
		int id = 0;
		getline(file, line);
		if(line == "")
		{
			continue;
		}
		if(line == "{");
		{
			data newData;
			newData.id = id++;
			int size = 0;
			getline(file, line);
			while(line != "}")
			{
				size++;
				newData.numbers.push_back(convertStringToInt(line));
				getline(file, line);
			}
			newData.size = size;
			dataBase.push_back(newData);
		}
	}

	file.close();
}

void setSort(vector<sort> &sortBase)
{
	sortBase.clear();
	string line;
	ifstream file;
	file.open("sort.txt");

	if(!file.good())
	{
		file.close();

		generateSort();

		file.open("sort.txt");
	}

	while(!file.eof())
	{
		getline(file, line);
		if(line == "")
		{
			continue;
		}
		if(line == "{")
		{
			sort newSort;
			getline(file, line);
			newSort.sortId = convertStringToInt(line);
			getline(file, line);
			newSort.id = convertStringToInt(line);
			getline(file, line);
			newSort.size = convertStringToInt(line);
			getline(file, line);
			newSort.type = line;
			getline(file, line);
			newSort.time = stod(line);
			sortBase.push_back(newSort);
		}
	}

	file.close();
}

void show(vector<data> &dataBase, vector<sort> &sortBase)
{
	int index = -1;
	bool end = false;
	while(index != 0)
	{
		system("CLS");
		cout<<"===================="<<endl
			<<"Wyswietlanie zestawu"<<endl
			<<"===================="<<endl;
		cout<<"Wybierz zestaw:"<<endl;
		for(int i = 1; i <= dataBase.size(); i++)
		{
			cout<<i<<". Zestaw nr "<<i<<"."<<endl;
		}
		cout<<"0. Powrot"<<endl;
		cout<<"-> "; cin>>index;
		if(index < 0 || index > dataBase.size())
		{
			error();
			continue;
		}
		if(index == 0)
			continue;

		int option = -1;
		while(option != 0)
		{
			system("CLS");
			cout<<"===================="<<endl
				<<"Wyswietlanie zestawu"<<endl
				<<"===================="<<endl;
			cout<<"Zestaw nr "<<index<<", wyswietl: "<<endl
				<<"1. Dane"<<endl
				<<"2. Wyniki sortowan"<<endl
				<<"0. Powrot"<<endl;
			cout<<"-> "; cin>>option;

			switch(option)
			{
				case 0:
					continue;
				case 1:
					showData(dataBase, index-1);
					break;
				case 2:
					showSort(sortBase, index, index-1);
					break;
				default:
					error();
					continue;
			}
		}
	}
}

void showData(vector<data> &dataBase, int index)
{
	system("CLS");
	cout<<"===================="<<endl
		<<"Wyswietlanie zestawu"<<endl
		<<"===================="<<endl;
	cout<<"Zestaw nr "<<index<<", dane: "<<endl;
	for(int i = 0; i < dataBase[index].numbers.size(); i++)
	{
		cout<<dataBase[index].numbers[i]<<endl;
	}
	system("pause");
}

void showSort(vector<sort> &sortBase, int index, int id)
{
	int n = 0;
	vector<sort> tempBase;
	system("CLS");
	cout<<"===================="<<endl
		<<"Wyswietlanie zestawu"<<endl
		<<"===================="<<endl;
	cout<<"Zestaw nr "<<index<<", wyniki sortowan: "<<endl;
	for(int i = 0; i < sortBase.size(); i++)
	{
		if(id == sortBase[i].id)
        {
            n++;
            tempBase.push_back(sortBase[i]);
        }
	}
	if(n == 0)
		cout<<"Brak sortowan"<<endl;
    else
    {
        double results[n];
        for(int i = 0; i < n; i++)
        {
            results[i] = tempBase[i].time;
        }
        quickSortRankingList(results, 0, n-1);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(tempBase[j].time == results[i])
                {
                    double seconds = tempBase[j].time;
                    double miliseconds = tempBase[j].time*pow(10, 3);
                    if(seconds >= 1)
                        cout<<"Typ sortowania: "<<tempBase[j].type<<", czas sortowania: "<<seconds<<" sekund"<<endl;
                    else
                        cout<<"Typ sortowania: "<<tempBase[j].type<<", czas sortowania: "<<miliseconds<<" milisekund"<<endl;
                }
            }
        }

    }
	system("pause");
}

void generateData(int n, int maxValue, int nSize)
{
	ofstream file;
	file.open("data.txt", ios::app);
	srand(time(NULL));

	for(int i = 1; i <= n; i++)
	{
		file<<"{"<<endl;
		int size;
		if(nSize == 0)
        {
            size = rand()%10000+95001;
        }
        else
        {
            size = nSize;
        }

		for(int index = 0; index < size; index++)
		{
		    if(maxValue == 0)
            {
                file<<rand()%1000000<<endl;
            }
            else
            {
                file<<rand()%maxValue<<endl;
            }
		}
		file<<"}"<<endl;
	}

	file.close();
}

void generateSort()
{
	ofstream file;
	file.open("sort.txt");
	file.close();
}

int convertStringToInt(string text)
{
	int length = text.length();
	int result = 0;
	int power = 0;

	for(int i = length-1; i >= 0; i--)
	{
		result += (int(text[i]) - 48) * pow(10, power);
		power++;
	}

	return result;
}

void addData(vector<data> &dataBase)
{
	int n;
	int maxValue, nSize;
	data newData;
	ofstream file;
	file.open("data.txt", ios::app);

	system("CLS");
	cout<<"========================"<<endl
		<<"Dodawanie zestawu danych"<<endl
		<<"========================"<<endl;
    cout<<"Podaj maksymalna wartosc dla zestawu: ";
    cin>>maxValue;
    cout<<"Podaj ilosc danych dla zestawu: ";
    cin>>nSize;
    generateData(1, maxValue, nSize);
    _sleep(2000);

	system("CLS");
	cout<<"============================"<<endl
		<<"Pomyslnie dodano nowy zestaw"<<endl
		<<"============================"<<endl;
	_sleep(2000);

	file.close();
}

void sortData(vector<data> &dataBase, vector<sort> &sortBase)
{
	int index = -1;
	sort newSort;
	ofstream file;
	file.open("sort.txt", ios::app);

	while(index != 0)
	{
		system("CLS");
		cout<<"=============="<<endl
			<<"Panel sortowan"<<endl
			<<"=============="<<endl;
		cout<<"Wybierz zestaw:"<<endl;
		for(int i = 1; i <= dataBase.size(); i++)
		{
			cout<<"Nr "<<i<<"."<<endl;
		}
		cout<<"0. Powrot"<<endl;
		cout<<"-> "; cin>>index;
		if(index < 0 || index > dataBase.size())
		{
			error();
			continue;
		}
		if(index == 0)
			continue;

		--index;

		newSort.id = index;

		int option = -1;
		while(option != 0)
		{
			system("CLS");
			cout<<"=============="<<endl
				<<"Panel sortowan"<<endl
				<<"=============="<<endl;
			cout<<"Wybierz rodzaj sortowania:"<<endl
				<<"1. mergeSort"<<endl
				<<"2. quickSort"<<endl
				<<"3. bucketSort"<<endl
				<<"4. bubbleSort"<<endl
				<<"5. insertSort"<<endl
				<<"0. Powrot"<<endl
				<<"-> "; cin>>option;

			if(option < 0 || option > 5)
			{
				error();
				continue;
			}

			Clock_val start, end;

			int size = dataBase[index].size;
			newSort.size = size;
			int *tab = new int [size];
			for(int i = 0; i < size; i++)
			{
				tab[i] = dataBase[index].numbers[i];
			}

			switch(option)
			{
				case 0:
				{
					continue;
				}
				case 1:
				{
					newSort.type = "mergeSort";
					int *supTab = new int [size];
					start = Clock::now();
					mergeSort(tab, 0, size-1, supTab);
					end = Clock::now();
					delete [] supTab;
					break;
				}
				case 2:
				{
					newSort.type = "quickSort";
					start = Clock::now();
					quickSort(tab, 0, size-1);
					end = Clock::now();
					break;
				}
				case 3:
				{
					newSort.type = "bucketSort";
					start = Clock::now();
					bucketSort(tab, size);
					end = Clock::now();
					break;
				}
				case 4:
				{
					newSort.type = "bubbleSort";
					start = Clock::now();
					bubbleSort(tab, size);
					end = Clock::now();
					break;
				}
				case 5:
				{
					newSort.type = "insertSort";
					start = Clock::now();
					insertSort(tab, size);
					end = Clock::now();
					break;
				}
				default:
					error();
			}

			if(sortBase.size() == 0)
				newSort.sortId = 0;
			else
				newSort.sortId = sortBase.size();

			delete [] tab;

			system("CLS");
			cout<<"=============="<<endl
				<<"Panel sortowan"<<endl
				<<"=============="<<endl;

			auto miliseconds = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count())*pow(10, -6);
			auto seconds = (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count())*pow(10, -9);

			if(seconds >= 1)
				cout<<"Czas sortowania "<<newSort.type<<": "<<seconds<<" sekund"<<endl;
			else
				cout<<"Czas sortowania "<<newSort.type<<": "<<miliseconds<<" milisekund"<<endl;
			newSort.time = double(seconds);

			sortBase.push_back(newSort);

			file<<"{"<<endl
				<<newSort.sortId<<endl
				<<newSort.id<<endl
				<<newSort.size<<endl
				<<newSort.type<<endl
				<<newSort.time<<endl
				<<"}"<<endl;

			system("pause");
		}
	}
	file.close();
}

void mergeSort(int *tab, int leftIndex, int rightIndex, int *supTab)
{
	if(leftIndex != rightIndex)
	{
		int pivot = (leftIndex + rightIndex) / 2;
		mergeSort(tab, leftIndex, pivot, supTab);
		mergeSort(tab, pivot + 1, rightIndex, supTab);
		merge(tab, leftIndex, pivot, rightIndex, supTab);
	}
}

void merge(int *tab, int leftIndex, int pivot, int rightIndex, int *supTab)
{
	for(int i = 0; i<= rightIndex; i++)
	{
		supTab[i] = tab[i];
	}

	int leftSideIndex = leftIndex;
	int rightSideIndex = pivot + 1;
	int index = leftIndex;

	while(leftSideIndex <= pivot && rightSideIndex <= rightIndex)
	{
		if(supTab[leftSideIndex] <= supTab[rightSideIndex])
		{
			tab[index] = supTab[leftSideIndex];
			leftSideIndex++;
		}
		else
		{
			tab[index] = supTab[rightSideIndex];
			rightSideIndex++;
		}
		index++;
	}

	while(leftSideIndex <= pivot)
	{
		tab[index] = supTab[leftSideIndex];
		index++;
		leftSideIndex++;
	}
}

void quickSort(int *tab, int left, int right)
{
	int i = left;
	int j = right;
	int v = tab[(left+right)/2];
	int x;
	do
	{
		while(tab[i] < v) i++;
		while(tab[j] > v) j--;
		if(i <= j)
		{
			x = tab[i];
			tab[i] = tab[j];
			tab[j] = x;
			i++;
			j--;
		}
	}
	while(i <= j);
	if(i < right) quickSort(tab, i, right);
	if(j > left) quickSort(tab, left, j);
}

void bucketSort(int *tab, int size)
{
	int min = 0;
	for(int i = 0; i < size; i++)
	{
		if(tab[i] < min)
			min = tab[i];
	}
	int neg = (min*-1);

	if(neg != 0)
	{
		for(int i = 0; i < size; i++)
		{
			tab[i] += neg;
		}
	}

	int max = 0;
	for(int i = 0; i < size; i++)
	{
		if(tab[i] > max)
			max = tab[i];
	}

	int *buckets = new int [max+1];

	for(int i = 0; i <= max; i++)
		buckets[i] = 0;

	for(int i = 0; i < size; i++)
		buckets[tab[i]]++;

	for(int i = 0; i < size; i++)
		tab[i] = 0;

	int index = 0;
	for(int i = 0; i <= max; i++)
	{
		if(buckets[i] > 0)
		{
			for(int j = 0; j < buckets[i]; j++)
			{
				tab[index] = i - neg;
				index++;
			}
		}
	}

	delete [] buckets;
}

void bubbleSort(int *tab, int size)
{
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < size-i-1; j++)
		{
			if(tab[j] > tab[j+1])
			{
				int x = tab[j+1];
				tab[j+1] = tab[j];
				tab[j] = x;
			}
		}
	}
}

void insertSort(int *tab, int size)
{
	int pivot = 1;
	int x;

	do
	{
		if(tab[pivot] < tab[pivot-1])
		{
			for(int i = pivot; i > 0; i--)
			{
				x = tab[i];
				tab[i] = tab[i-1];
				tab[i-1] = x;
			}
		}
		pivot++;
	}
	while(pivot < size);
}

void showRankingList(vector<sort> &sortBase)
{
	int size = sortBase.size();
	int *sortIdCheck = new int [size];
	double *tab = new double [size];

	for(int i = 0; i < size; i++)
	{
		sortIdCheck[i] = true;
	}

	for(int i = 0; i < size; i++)
	{
		tab[i] = sortBase[i].time;
	}

	quickSortRankingList(tab, 0, size-1);

	system("CLS");
	cout<<"============================"<<endl
		<<"Wyswietlam ranking sortowan"<<endl
		<<"============================"<<endl;

	if(size > 0)
	{
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				if(tab[i] == sortBase[j].time && sortIdCheck[j] == true)
				{
					double seconds = sortBase[j].time;
					double miliseconds = sortBase[j].time*pow(10, 3);
					if(seconds >= 1)
						cout<<i+1<<". "<<"Czas: "<<seconds<<" sekund, typ: "<<sortBase[j].type<<", ilosc danych: "<<sortBase[j].size<<", zestaw nr. "<<sortBase[j].id+1<<endl;
					else
						cout<<i+1<<". "<<"Czas: "<<miliseconds<<" milisekund, typ: "<<sortBase[j].type<<", ilosc danych: "<<sortBase[j].size<<", zestaw nr. "<<sortBase[j].id+1<<endl;
					sortIdCheck[j] = false;
					break;
				}
			}
		}
	}
	else
	{
		cout<<"Brak sortowan"<<endl;
	}

	system("pause");

	delete [] tab;
	delete [] sortIdCheck;
}

void quickSortRankingList(double *tab, int left, int right)
{
	int i = left;
	int j = right;
	double v = tab[(left+right)/2];
	double x;
	do
	{
		while(tab[i] < v) i++;
		while(tab[j] > v) j--;
		if(i <= j)
		{
			x = tab[i];
			tab[i] = tab[j];
			tab[j] = x;
			i++;
			j--;
		}
	}
	while(i <= j);
	if(i < right) quickSortRankingList(tab, i, right);
	if(j > left) quickSortRankingList(tab, left, j);
}

void error()
{
	system("CLS");
	cout<<"===================="<<endl
		<<"\aNie ma takiej opcji!"<<endl
		<<"===================="<<endl;
	system("pause");
}
