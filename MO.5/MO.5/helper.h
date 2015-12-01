#pragma once
#include <iostream>
#include <string>

using std::string;
using std::ostream;

class SimplexA {
public:
	SimplexA(string file_name);
	~SimplexA();

	friend ostream& operator<<(ostream& stream, const SimplexA& a);
	void RtoK(int r, int k);
	void solve();

private:

	double** dataA;
	//double** dataB;

	int* free;
	int* basic;
	int n;
	int m;
};
class SimplexB {
public:
	SimplexB(string file_name);
	~SimplexB();

	friend ostream& operator<<(ostream& stream, const SimplexB& a);
	void RtoK(int r, int k);
	void solve();

private:

	double** dataB;
	//double** dataB;

	int* free;
	int* basic;
	int n;
	int m;
};