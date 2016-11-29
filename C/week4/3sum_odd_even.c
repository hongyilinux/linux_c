#include <iostream>
using namespace std;

//求输入正整数m n，0 <= m <= n <= 300 
//求m到n之间所有的奇数和，并输出

int main()
{

	cout << "please input tow positive integer and the max must less then 300" << endl;

	int  m,n;
	int sum = 0;
	int i;

	cout << "please input m" << endl;
	cin >> m;
	cout << "please input n" << endl;
	cin >> n;

	/*
	for(i = m ; i <= n ; i++)
	{
		if(i%2 == 1)
			sum += i;
	}

	*/

	while(m <= n)
	{	
		if(m%2 == 1)
		{
			sum += m;
			m+= 2;
		}else
		{
			m++;
			sum += m;
			m += 2;
		}
	}

	cout << "the result is "<< sum << endl;

	return 0;
}
