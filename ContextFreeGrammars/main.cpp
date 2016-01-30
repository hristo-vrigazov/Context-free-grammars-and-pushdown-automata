#include <iostream>
#include <string>
#include "Terminal.h"
#include "Variable.h"

using namespace std;

int main()
{
	string s("A"), v("B");
	cout << (s < v);
	return 0;
}