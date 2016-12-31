#include <stdio.h>
#include <stdlib.h>

int main()
{
	int x = -1;

	unsigned u = 2147483648;
	
	float f = -12.75;

	printf("x = %u = %d\n",x,x);
	printf("u = %u = %d\n",u,u);
	printf("f = %x\n",f);

	//注意浮点数除0 ，没有错误
	printf("f/0 = %f\n",f/0);

	if(-2147483648 < 2147483647)
		printf("-2147483648 < 2147483647 is true\n");
	else
		printf("-2147483648 < 2147483647 is fale\n");
	
	if(-2147483648 -1 < 2147483647)
		printf("-2147483648 -1 < 2147483647\n");
	else if(-2147483648 -1 < 2147483647)
		printf("-2147483648 -1 ==  2147483647\n");
	else
		printf("-2147483648 -1 >  2147483647\n");

	exit(0);
}


