#include <stdio.h>
#include <stdlib.h>

int main()
{
	int num;
	int i,j,flag;

	puts("please input a number");
	scanf("%d",&num);

	for(i = num ; i >=2 ; i--)
	{
		flag = 0;
		for(j = 2 ; j < i/2 ; j++)
		{
			if(i % j == 0 )
			{
				flag = 1;
				break;
			}
		}

		if(flag == 0)
		{
			printf("The max primer less then n is %d\n",i);
			break;
		}
	}



	exit(0);
}


