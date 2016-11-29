#include <iostream>
using namespace std;

static void print_arr(int arr[],int size)
{
	int i;

	for(i = 0 ; i < size ; i++)
	{
		cout << arr[i] << ' ' ;
	}

	cout << endl;
}


int main()
{
	int a[10];
	int i = 0;
	int j = 0;
	int tmp;
	bool leftIsEven;
	bool rightIsEven;
	
	cout << "please inpute ten intgers" << endl;

	for(i = 0 ; i < 10 ; i++)
		cin >> a[i];
	
	//冒泡，不断比较相邻的两个数，如果顺序错了就交换
	for(i = 0 ; i < 9 ; i++)
	{
		for(j = 1 ; j < 10 - i ; j++)
		{
			//与一般的冒泡不同，我们不只是通过不叫数字的大小决定顺序
			//如果左边为偶数，右边为奇数，那么书序也颠倒
			leftIsEven = a[j-1] % 2 == 0;		//取模运算
			rightIsEven = a[j] % 2 == 0;
			
			if((leftIsEven && !rightIsEven) || ((leftIsEven == rightIsEven) && a[j-1] > a[j]))
			{
				tmp = a[j-1];
				a[j-1] = a[j];
				a[j] = tmp;
			}
		}
		cout << "第 " << i << "遍结果" << endl;
		print_arr(a,10);
	}
	
	cout << "the result is "<< endl;
	print_arr(a,10);
}


