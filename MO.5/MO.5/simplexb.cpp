#include "helper.h"
#include <fstream>
#include <iomanip>

using std::ifstream;
using std::cout;
using std::setw;

SimplexB::SimplexB(string file_name) {

	ifstream file(file_name);

	file >> n >> m; //��������� ����������� �������

	dataB = new double*[m + 1]; /*������� ������ (m+1) x (n+1), �.�. ����� �����
								�������� ����� ������� ��������� ������ � ������ ������� �������*/

	for (int i = 0; i < m + 1; i++) {
		dataB[i] = new double[n + 1];
	}

	//�������� ������ �������� ������� ������� �� �����
	for (int j = 0; j < n; j++) {
		dataB[m][j + 1] = 1;
	}

	//��������� ������� �������� �� �����
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			file >> dataB[i][j + 1];
		}
	}

	//�������� ������ �������� ��������� ������
	for (int i = 0; i < m; i++) {
		dataB[i][0] = 1;
	}

	dataB[m][0] = 0;
	file.close();

	free = new int[n]; //��������� ����������
	for (int i = 0; i < n; i++) {
		free[i] = i + 1;
	}

	basic = new int[m]; //�������� ����������
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

	//����� �������� �������
	int iter = 1;
	bool loop = true;
	cout << "����� �������� �������\n";
	while (loop) {
		loop = false;

		for (int i0 = 0; i0 < m; i0++) {// ����� i0 ������
			if (dataB[i0][0] < 0) {
				int k = 0;
				for (int j = 1; j < n + 1 && !k; j++) { //����� k �������
					if (dataB[i0][j] < 0) {
						k = j;
					}
				}
				if (k) {
					loop = true;
					float min = FLT_MAX;
					int r = 0;
					for (int i = 0; i < m; i++) { //����� r ������
						if (dataB[i][0] / dataB[i][k] < min && dataB[i][0] / dataB[i][k] > 0) {
							r = i;
							min = dataB[i][0] / dataB[i][k];
						}
					}
					RtoK(r, k);
					break;
				}
				else {
					std::cout << "��� �������\n";
					return;
				}
			}
		}
		if (!loop) break;
		cout << iter++ << *this << "\n";
	}

	//����� ������������ �������
	loop = true;
	iter = 1;
	cout << "����� ������������ �������\n";
	while (loop) {
		loop = false;
		int r = 0, k = 0;
		for (int j = 1; j < n + 1; j++) { //����� k �������
			if (dataB[m][j] > 0)  {
				k = j;
				float minimum = FLT_MAX;
				for (int i = 0; i < 3; i++) { //����� r ������
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

	//����� ����������
	if (!is_solve) cout << "��� �������\n";
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
