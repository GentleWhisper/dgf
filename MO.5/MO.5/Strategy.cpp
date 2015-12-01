#include "helper.h"
using std::cout;
using std::cin;

void Strategy(double* solveA, double& fA, double* solveB, double& fB){
	double StrategyA[4] = { 0, 0, 0, 0 };
	double StrategyB[5] = { 0, 0, 0, 0, 0 };

	
	cout << "Стратегии игрока А:\n\n";
	for (int i = 0; i < 4; i++) {
		StrategyA[i] = solveA[i]*(1 / fA);
		cout << "Выбор " << i + 1 << "-ой стратегии игрока А: " << StrategyA[i] << "\n\n";
	}
	cout << "Стратегии игрока B:\n\n";
	for (int i = 0; i < 5; i++) {
		StrategyB[i] = solveB[i]*(1 / fB);
		cout << "Выбор " << i + 1 << "-ой стратегии игрока В: " << StrategyB[i] << "\n\n";
	}

};