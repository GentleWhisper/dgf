#include "helper.h"
using std::cout;
using std::cin;

void Strategy(double* solveA, double& fA, double* solveB, double& fB){
	double StrategyA[4] = { 0, 0, 0, 0 };
	double StrategyB[5] = { 0, 0, 0, 0, 0 };

	
	cout << "��������� ������ �:\n\n";
	for (int i = 0; i < 4; i++) {
		StrategyA[i] = solveA[i]*(1 / fA);
		cout << "����� " << i + 1 << "-�� ��������� ������ �: " << StrategyA[i] << "\n\n";
	}
	cout << "��������� ������ B:\n\n";
	for (int i = 0; i < 5; i++) {
		StrategyB[i] = solveB[i]*(1 / fB);
		cout << "����� " << i + 1 << "-�� ��������� ������ �: " << StrategyB[i] << "\n\n";
	}

};