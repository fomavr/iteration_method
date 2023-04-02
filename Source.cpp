#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

void readMatrix(vector<vector<int>>& matr, string filename) {
	ifstream fin(filename + ".txt");

	if (fin.is_open()){
		for (size_t i = 0; i < matr.size(); i++)
			for (size_t j = 0; j < matr.at(0).size(); j++)
				fin >> matr.at(i).at(j);
	}

	fin.close();
}

void printMatrix(const vector<vector<int>> matr) {
	for (size_t i = 0; i < matr.size(); i++) {
		for (size_t j = 0; j < matr.at(0).size(); j++)
			cout << matr.at(i).at(j) << "\t";
		cout << endl;
	}
}

bool findSedlDot(const vector<vector<int>> matr) {
	vector<int> min;
	vector<int> max;
	//ищем минмальный элемент по строкам
	for (size_t i = 0; i < matr.size(); i++){
		auto min_rows = min_element(matr.at(i).cbegin(), matr.at(i).cend());
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


void alg(vector<vector<int>> matr) {
	int iter = 8;//количество итераций алгоритма в будущем будет ручной ввод
	int i = 3;//строка массива, с которой начинаем алгоритм (в будущем тоже будет ручной ввод)

	vector<vector<int>> B = {};
	vector<vector<int>> A = {};

	vector<double> min = {};
	vector<double> max = {};

	map<int, int> Sa = {};
	map<int, int> Sb = {};



	B.resize(iter);
	A.resize(iter);
	
	int j = 0;//указатель на текущий номер итерации

	while (iter != 0){

		copy(begin(matr.at(i)), end(matr.at(i)), back_inserter(B.at(j)));//копируем i-строку для начала алгоритма
		
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
		auto search = Sb.find(i);
		if (Sb.find(i) == Sb.end())
			Sb.insert(make_pair(i, 1));
		else 	
			search->second++;

		
		min.push_back((double)cur_min/(j + 1));

		
		//работаем со столбцами
		for (size_t k = 0; k < A.at(i).size(); k++)
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

		search = Sa.find(i);

		if (Sa.find(i) == Sa.end())
			Sa.insert(make_pair(i, 1));
		else
			search->second++;

		max.push_back((double)cur_max/(j + 1));

		j++;

		iter--;

	}

	auto max_min = *max_element(min.cbegin(), min.cend());
	auto min_max = *min_element(max.cbegin(), min.cend());

	vector<double> k = {};

	for (size_t h = 0; h < min.size(); h++)
		k.push_back(min.at(h) - max.at(h));

	auto min_delta = *min_element(k.cbegin(), k.cend());
	double gamma = (min_max + max_min) / 2;
}


int main() {

	vector<vector<int>> matrix = {};

	int M = 9;
	int N = 9;

	matrix.resize(M);
	for (auto& cur_vec : matrix)
		cur_vec.resize(N);
	
	readMatrix(matrix, "temp");
	printMatrix(matrix);

	cout << findSedlDot(matrix) << endl;

	alg(matrix);

	
	return 0;
}

