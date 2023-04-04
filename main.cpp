#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

void readMatrix(vector<vector<int>>& matr, const string& filename) {
	ifstream fin(filename + ".txt");

	if (fin.is_open()){
		for (size_t i = 0; i < matr.size(); i++)
			for (size_t j = 0; j < matr.at(0).size(); j++)
				fin >> matr.at(i).at(j);
	}

	fin.close();
}

void printMatrix(const vector<vector<int>>& matr) {
	for (size_t i = 0; i < matr.size(); i++) {
		for (size_t j = 0; j < matr.at(0).size(); j++)
			cout << matr.at(i).at(j) << "\t";
		cout << endl;
	}
}

bool findSedlDot(const vector<vector<int>>& matr) {
	vector<int> min;
	vector<int> max;

	//ищем минмальный элемент по строкам
	for (const auto & i : matr){
		auto min_rows = min_element(i.cbegin(), i.cend());
		min.push_back(*min_rows);
	}

	//ищем максимальный элемент по столбцам

	for (int j = 0; j < matr.size(); j++){
		int max_col = matr.at(0).at(j);
		for (int i = 0; i < matr.at(0).size(); i++)
			if (matr.at(i).at(j) > max_col)
				max_col = matr.at(i).at(j);
		max.push_back(max_col);
	}

	//здесь мы сравниваем максимальный среди минимальных и минимальный среди максимальных
	return *min_element(max.cbegin(), max.cend()) == *max_element(min.cbegin(), min.cend());
}


void iteration_method(vector<vector<int>> matr) {
	int iter = 8;//количество итераций алгоритма в будущем будет ручной ввод

    cout << "Введите количество итераций алгоритма: ";
    cin >> iter;
	
	//строка массива, с которой начинаем алгоритм
	//должна быть небольше длины строки исходной матрицы
	int i = 3;

    cout << endl << "Введите строку массива, с которой начнётся алгоритм:";
    do {
        cin >> i;
    } while (i >= matr.size());

    i--;//уменьшаем на единицу, потому что нумерация идёт с нуля

    vector<vector<int>> B = {};
	vector<vector<int>> A = {};

	vector<double> min = {};
	vector<double> max = {};

	map<int, int> Sa = {};
	map<int, int> Sb = {};

	B.resize(iter);
	for (auto& cur_vec : B)
		cur_vec.resize(matr.at(0).size());

	A.resize(iter);
	for (auto& cur_vec : A)
		cur_vec.resize(matr.at(0).size());
	
	int j = 0;//указатель на текущий номер итерации

	while (iter != 0){

		B.at(j) = matr.at(i);

		//если итерация не первая, то прибавляем к нашей строке резултат предыдущей итерации
		if (j != 0){
			for (size_t k = 0; k < B.at(j).size(); k++)
				B.at(j).at(k) += B.at(j - 1).at(k);
		}

		//находим минимальный элемент в строчек и сохраняем его индекс
		int cur_min = B.at(j).at(0);
		i = 0;
		for (int k = 0; k < B.at(j).size(); k++) {
			if (cur_min > B.at(j).at(k)){
				cur_min = B.at(j).at(k);
				i = k;
			}
		}

		//записываем индекс минимального элемента для конца решения или увеличивае счётчик, если такой индекс уже имеется
		auto search = Sb.find(i + 1);
		if (search == Sb.end())
			Sb.insert(make_pair(i + 1, 1));
		else 	
			search->second++;

		
		min.push_back((double)cur_min/(j + 1));

		
		//работаем со столбцами
		for (size_t k = 0; k < A.at(0).size(); k++)
			A.at(j).at(k) = matr.at(k).at(i);
		
		if (j != 0) {
			for (size_t k = 0; k < A.at(j).size(); k++)
				A.at(j).at(k) += A.at(j - 1).at(k);
		}

		int cur_max = A.at(j).at(0);
		i = 0;
		for (int k = 0; k < A.at(j).size(); k++) {
			if (cur_max < A.at(j).at(k)) {
				cur_max = A.at(j).at(k);
				i = k;
			}
		}

		//добавляем для ясности единицу
		search = Sa.find(i + 1);

		if (search == Sa.end())
			Sa.insert(make_pair(i + 1, 1));
		else
			search->second++;

		max.push_back((double)cur_max/(j + 1));

		j++;
		iter--;

	}

	auto max_min = *max_element(min.cbegin(), min.cend());
	auto min_max = *min_element(max.cbegin(), max.cend());

	vector<double> k = {};

	for (size_t h = 0; h < min.size(); h++)
		k.push_back(abs(min.at(h) - max.at(h)));

	auto min_delta = *min_element(k.cbegin(), k.cend());
	double gamma = (min_max + max_min) / 2;


    cout << "\nРешение игры\n";
    cout << "Матрица B\n";
    printMatrix(B);
    cout << "Матрица A\n";
    printMatrix(A);
    cout << "Гамма: " << gamma << endl;
    cout << "Дельта k: " << min_delta << endl;
    cout << "Стратегии A:\n";
    for (const auto& strateg: Sa)
        cout << "A" << strateg.first << "\t";
    cout << endl;
    for (const auto& strateg: Sa)
        cout << strateg.second << "/8" << "\t";
    cout << endl;

    cout << "Стратегии B:\n";
    for (const auto& strateg: Sb)
        cout << "B" << strateg.first << "\t";
    cout << endl;

    for (const auto& strateg: Sb)
        cout << strateg.second << "/8" << "\t";
    cout << endl;


}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
	vector<vector<int>> matrix = {};

	int M = 9;//строки
	int N = 9;//столбцы

    cout << "Введите количество строк:";
    cin >> M;

    cout << endl << "Введите количество столбцов матрицы:";
    cin >> N;
    cout << endl;

	matrix.resize(M);
	for (auto& cur_vec : matrix)
		cur_vec.resize(N);
	
	readMatrix(matrix, "temp");
	printMatrix(matrix);

	cout << findSedlDot(matrix) << endl;

    if (findSedlDot(matrix)){
        cout << "Матрица имеет седловую точку!\n";
        return -1;
    }

    cout << "Матрица не имеет седловой точки\n";


    cout << "Решаем матрицу итеративным методом\n";
    iteration_method(matrix);

	return 0;
}

