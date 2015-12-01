#include "helper.h"
#include <fstream>
#include <iomanip>

using std::ifstream;
using std::cout;
using std::setw;

SimplexB::SimplexB(string file_name) {

	ifstream file(file_name);

	file >> n >> m; //Считываем размерность матрицы

	dataB = new double*[m + 1]; /*Создаем массив (m+1) x (n+1), т.к. кроме самих
								значений нужен столбец свободных членов и строка целевой функции*/

	for (int i = 0; i < m + 1; i++) {
		dataB[i] = new double[n + 1];
	}

	//Считыаем вектор значений целевой функции из файла
	for (int j = 0; j < n; j++) {
		dataB[m][j + 1] = 1;
	}

	//Считываем матрицу значений из файла
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			file >> dataB[i][j + 1];
		}
	}

	//Считыаем вектор значений свободных членов
	for (int i = 0; i < m; i++) {
		dataB[i][0] = 1;
	}

	dataB[m][0] = 0;
	file.close();

	free = new int[n]; //Свободные переменные
	for (int i = 0; i < n; i++) {
		free[i] = i + 1;
	}

	basic = new int[m]; //Базисные переменные
	for (int i = 0; i < m; i++) {
		basic[i] = n + i + 1;
	}
}

SimplexB::~SimplexB() {

	for (int i = 0; i < m + 1; i++) {
		delete[] dataB[i];
	}
	delete[] dataB;
	delete[] free;
	delete[] basic;
}

void SimplexB::RtoK(int r, int k) {

	for (int i = 0; i < m + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			if (i != r && j != k) {
				dataB[i][j] = dataB[i][j] - dataB[i][k] * dataB[r][j] / dataB[r][k];
			}
		}
	}

	for (int i = 0; i < m + 1; i++) {
		if (i != r) {
			dataB[i][k] = -dataB[i][k] / dataB[r][k];
		}
	}

	for (int j = 0; j < n + 1; j++) {
		if (j != k) {
			dataB[r][j] = dataB[r][j] / dataB[r][k];
		}
	}

	dataB[r][k] = 1.0 / dataB[r][k];

	int i = free[k - 1];
	free[k - 1] = basic[r];
	basic[r] = i;
}

void SimplexB::solve() {
	bool is_solve = true;

	//Поиск опорного решения
	int iter = 1;
	bool loop = true;
	cout << "Поиск опорного решения\n";
	while (loop) {
		loop = false;

		for (int i0 = 0; i0 < m; i0++) {// поиск i0 строки
			if (dataB[i0][0] < 0) {
				int k = 0;
				for (int j = 1; j < n + 1 && !k; j++) { //поиск k столбца
					if (dataB[i0][j] < 0) {
						k = j;
					}
				}
				if (k) {
					loop = true;
					float min = FLT_MAX;
					int r = 0;
					for (int i = 0; i < m; i++) { //поиск r строки
						if (dataB[i][0] / dataB[i][k] < min && dataB[i][0] / dataB[i][k] > 0) {
							r = i;
							min = dataB[i][0] / dataB[i][k];
						}
					}
					RtoK(r, k);
					break;
				}
				else {
					std::cout << "Нет решения\n";
					return;
				}
			}
		}
		if (!loop) break;
		cout << iter++ << *this << "\n";
	}

	//Поиск оптимального решения
	loop = true;
	iter = 1;
	cout << "Поиск оптимального решения\n";
	while (loop) {
		loop = false;
		int r = 0, k = 0;
		for (int j = 1; j < n + 1; j++) { //поиск k столбца
			if (dataB[m][j] > 0)  {
				k = j;
				float minimum = FLT_MAX;
				for (int i = 0; i < 3; i++) { //поиск r строки
					if (dataB[i][j] > 0 && dataB[i][0] / dataB[i][k] < minimum) {
						r = i;
						minimum = dataB[i][0] / dataB[i][k];
					}
				}

				if (minimum < FLT_MAX) {
					loop = true;
					RtoK(r, k);
					break;
				}
				else {
					is_solve = false;
					break;
				}
			}
		}
		if (!loop) break;
		cout << iter++ << *this << "\n";
	}

	//Вывод результата
	if (!is_solve) cout << "Нет решения\n";
	else {
		for (int i = 0; i < m/* - (m - n)*/; i++) {
			cout << "X" << i + 1 << " = ";
			for (int j = 0; j < n; j++) {
				if (free[j] == i + 1) {
					cout << "0\n";
				}
				if (basic[j] == i + 1) {
					cout << dataB[j][0] << "\n";
				}
			}
		}
		std::cout << "F  = " << -dataB[m][0] << "\n";
	}
}

ostream& operator<<(ostream& stream, const SimplexB& a) {
	stream << setw(16) << "";
	for (int i = 0; i < a.n; i++) {
		stream << setw(8) << "X" << a.free[i] << "\t";
	}
	stream << "\n\n";
	for (int i = 0; i < a.m + 1; i++) {
		if (i == a.m) {
			stream << "    ";
		}
		else {
			stream << "X" << a.basic[i] << "  ";
		}
		for (int j = 0; j < a.n + 1; j++) {
			stream << setw(8) << a.dataB[i][j] << "\t";
		}
		stream << "\n";
	}
	return stream;
