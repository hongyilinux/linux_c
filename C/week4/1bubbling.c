#include <iostream>
using namespace std;

int main()
{
	int n,a[1000];

	cout << "please input N :" << endl;
	cin >> n;

	cout << "please intput array: " << endl;
	for(int i = 0 ; i < n ; i++)
		cin >> a[i];

	//冒泡法排序
	for(int i = 0; i < n-1 ; i++)
	{
		for(int j = 1; j < n-i ; j++)
		{
			if(a[j - 1] > a[j])
			{
				int tmp = a[j];
				a[j] = a[j-1];
				a[j-1] = tmp;
			}
		}
	}

	//依次输出排序后的结果
	for(int i = 0 ; i < n ; i++)
		cout << a[i] << endl;
	
	return 0;

}
