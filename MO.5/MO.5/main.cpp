#include "helper.h"
#include <locale>

using std::cout;
using std::cin;

int main() {
	setlocale(0, "rus");
	cout << "ֲגוהטעו טלÿ פאיכא\n";
	string input;
	cin >> input;
	double Fa, Fb;
	double solveA[5], solveB[4];
	double strategyA[5], strategy[4];

	/*SimplexA simplexA(input);
	std::cout << simplexA << "\n";
	simplexA.solve();
	*/

	SimplexB simplexB(input);
	std::cout << simplexB << "\n";
	simplexB.solve();


	system("pause");
	return 0;
}