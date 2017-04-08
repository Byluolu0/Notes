#include "strblob.h"
#include <iostream>
#include <vector>
#include <cassert>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

void test_strblob()
{
	StrBlob sb1;
	sb1.push_back("a");
	StrBlob sb2(sb1);
}

vector<int>* newIntVector()
{
	return new vector<int>();
}

void cinIntVector(vector<int>* vec)
{
	assert(vec != nullptr);
	int temp = 0;
	while (cin >> temp)
		vec->push_back(temp);
}

void coutIntVector(vector<int>* vec)
{
	assert(vec != nullptr);
	for (vector<int>::iterator it = vec->begin(); it != vec->end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void test_12_6()
{
	vector<int>* vec = newIntVector();
	cinIntVector(vec);
	coutIntVector(vec);
	delete vec;
}

int main()
{
	//test_strblob();
	test_12_6();
	return 0;
}