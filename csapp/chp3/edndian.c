#include <stdio.h>
#include <stdlib.h>
/*
 *	测试数据是大端还是小端存放的
 *
 */

void main()
{
	union NUM
	{
		int a;
		char b;
	} num;

	num.a = 0x12345678;

	if(num.b = 0x12)
		printf("Big Endian \n");
	else
		printf("Little Endian \n");
	
	printf("num.b = 0x%X \n",num.b);


	exit(0);

}


