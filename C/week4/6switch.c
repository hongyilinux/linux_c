#include <iostream>
using namespace std;

int main()
{
	int score,num;

	cout << "please give the score" << endl;

	cin >> score;

	num = score/10;	// 注意两个整数相除，结果还是整数

	switch(num)
	{
		case 10:
			cout << "A" << endl;
			break;
		case 9:
			cout << "B" << endl;
			break;
		case 8:
			cout << "C" << endl;
			break;
		case 7:
			cout << "D" << endl;
			break;
		default:
			cout << "wrong number" << endl;
			break;
	}

	return 0;
}


