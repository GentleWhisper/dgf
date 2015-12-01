#include "helper.h"
#include <fstream>
#include <iomanip>

using std::ifstream;
using std::cout;
using std::setw;

SimplexA::SimplexA(string file_name) {

	ifstream file(file_name);

	//Размеры матрицы значений ограничений
	file >> n >> m;

	dataA = new double*[m + 1];
	for (int i = 0; i < m + 1; i++) {
		dataA[i] = new double[n + 1];
	}

	//Считываем вектор значений свобоных членов
	for (int i = 0; i < m; i++) {
		dataA[i][0] = -1;
	}

	//Считываем матрицу значений
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < m; i++) {
			file >> dataA[i][j + 1];
			dataA[i][j + 1] = -dataA[i][j + 1];
		}
	}

	//Считываем вектор значений функции
	for (int j = 0; j < n; j++) {
		dataA[m][j + 1] = -1;
	}
	dataA[m][0] = 0;
	file.close();

	free = new int[n];
	for (int i = 0; i < n; i++) {
		free[i] = i + 1;
	}

	basic = new int[m];
	for (int i = 0; i < m; i++) {
		basic[i] = n + i + 1;
	}
}

SimplexA::~SimplexA() {

	for (int i = 0; i < m + 1; i++) {
		delete[] dataA[i];
	}
	delete[] dataA;
	delete[] free;
	delete[] basic;
}

void SimplexA::RtoK(int r, int k) {

	for (int i = 0; i < m + 1; i++) {
		for (int j = 0; j < n + 1; j++) {
			if (i != r && j != k) {
				dataA[i][j] = dataA[i][j] - dataA[i][k] * dataA[r][j] / dataA[r][k];
			}
		}
	}

	for (int i = 0; i < m + 1; i++) {
		if (i != r) {
			dataA[i][k] = -dataA[i][k] / dataA[r][k];
		}
	}

	for (int j = 0; j < n + 1; j++) {
		if (j != k) {
			dataA[r][j] = dataA[r][j] / dataA[r][k];
		}
	}

	dataA[r][k] = 1.0 / dataA[r][k];

	int i = free[k - 1];
	free[k - 1] = basic[r];
	basic[r] = i;
}

void SimplexA::solve() {
	bool solve = true;

	//Поиск опорного решения
	int iter = 1;
	bool recurse = true;
	cout << "Поиск опорного решения\n";
	while (recurse) {
		recurse = false;

		for (int i0 = 0; i0 < m; i0++) {// поиск i0 строки
			if (dataA[i0][0] < 0) {
				int k = 0;
				for (int j = 1; j < n + 1 && !k; j++) { //поиск k столбца
					if (dataA[i0][j] < 0) {
						k = j;
					}
				}
				if (k) {
					recurse = true;
					float min = FLT_MAX;
					int r = 0;
					for (int i = 0; i < m; i++) { //поиск r строки
						if (dataA[i][0] / dataA[i][k] < min && dataA[i][0] / dataA[i][k] > 0) {
							r = i;
							min = dataA[i][0] / dataA[i][k];
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
		if (!recurse) break;
		cout << iter++ << " Итерация\n" << *this << "\n";
	}

	//Поиск оптимального решения
	recurse = true;
	iter = 1;
	cout << "Поиск оптимального решения\n";
	while (recurse) {
		recurse = false;
		int r = 0, k = 0;
		for (int j = 1; j < n + 1; j++) { //поиск k столбца
			if (dataA[m][j] > 0)  {
				k = j;
				float minimum = FLT_MAX;
				for (int i = 0; i < 3; i++) { //поиск r строки
					if (dataA[i][j] > 0 && dataA[i][0] / dataA[i][k] < minimum) {
						r = i;
						minimum = dataA[i][0] / dataA[i][k];
					}
				}

				if (minimum < FLT_MAX) {
					recurse = true;
					RtoK(r, k);
					break;
				}
				else {
					solve = false;
					break;
				}
			}
		}
		if (!recurse) break;
		cout << iter++ << " Итерация\n" << *this << "\n";
	}

	//Вывод решения
	if (!solve) cout << "Нет решения\n";
	else {
		for (int i = 0; i < m; i++) {
			cout << "X" << i + 1 << " = ";
			for (int j = 0; j < n; j++) {
				if (free[j] == i + 1) {
					cout << "0\n";
				}
				if (basic[j] == i + 1) {
					cout << dataA[j][0] << "\n";
				}
			}
		}
		std::cout << "F  = " << dataA[m][0] << "\n";
	}
}

ostream& operator<<(ostream& stream, const SimplexA& a) {
	stream << setw(16) << "";
	for (int i = 0; i < a.n; i++) {
		stream << setw(8) << "X" << a.free[i] << "\t";
	}
	stream << "\n";
	for (int i = 0; i < a.m + 1; i++) {
		if (i == a.m) {
			stream << "    ";
		}
		else {
			stream << "X" << a.basic[i] << "  ";
		}
		for (int j = 0; j < a.n + 1; j++) {
			stream << setw(8) << a.dataA[i][j] << "\t";
		}
		stream << "\n";
	}
	return stream;
}