#include <iostream>
using namespace std;

int main()
{
	int a = -123;
	
	int b = 0x7ffff85;

	cout << hex << a << endl;
	cout << oct << a << endl;
	
	cout << dec << b << endl;

	return 0;
}

