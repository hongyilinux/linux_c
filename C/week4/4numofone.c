#include <iostream>
using namespace std;
/*
 * 	给定一个十进制整数N，求其对应2进制数中1的个数
 *
 *
 */

int main()
{
	int decimal;
	int count = 0;

	cin >> decimal;

	while(decimal)
	{
		count++;
		decimal = decimal & (decimal -1);
	}

	cout << "the result is" << count << endl;
	
	return 0;
}
