#include <iostream>
using namespace std;

int main()
{
	int a[10];

	cout<<"please input ten integer"<<endl;

	for(int i = 0 ; i < 10 ; i++)
		cin >> a[i];

	//首先，把奇数放在数组的左边，把偶数放在数组的有变
	int l = 0;
	int r = 9;

	while(l <= r)
	{
		//判断奇数还是偶数
		bool leftIsOdd = a[l]%2 == 1;
		bool rightIsEven = a[r]%2 == 0;

		if(leftIsOdd)
			l++;
		else if(rightIsEven)
			r--;
		//如果左侧不是奇数，右侧不是偶数，满足条件，交换位置，循环结束，左侧全是奇数，右侧全是偶数，且找到了位置，l
		else if(!leftIsOdd && !rightIsEven)
		{
			int tmp = a[l];
			a[l] = a[r];
			a[r] = tmp;
			l++;
			r--;
		}
	}

	cout << "l is "<< l << endl;
	cout << "r is "<< r << endl;

	//用冒泡法排序，l左侧的奇数部分
	int start = 0 , end = l;

	for(int i = start ; i < end -1; i++)
	{
		for(int j = start+1 ; j < start + end - i ; j++)
		{
			if(a[j-1] > a[j])
			{
				int tmp = a[j-1];
				a[j-1] = a[j];
				a[j] = tmp;
			}
		}
	}

	//用冒泡法派逊，对l右侧的偶数部分排序
	start = l;
	end = 10;

	for(int i = start ; i < end -1 ; i++)
	{
		for(int j = start + 1 ; j < start + end - i ;j++)
		{
			if(a[j-1] > a[j])
			{
				int tmp = a[j-1];
				a[j-1] = a[j];
				a[j] = tmp;
			}
		}
	}


	for(int i = 0 ; i < 10 ; i++)
	{
		cout << a[i] << endl;
	}
}
