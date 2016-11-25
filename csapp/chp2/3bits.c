#include <stdio.h>
#include <stdlib.h>

//与，或，非，异或的用法
//注意写法的通用型，和可移植性

int main()
{

	int a = 0x87654321;


	//掩码运算法则
	int x = 0xFF;

	printf("%x\n",a&x);

	//最低有效字节不变，其他字节都取补,这样写不具有移植性
	//int y= 0xFFFFFF00;
	//printf("%x\n",a^y);

	int y = ~0xFF;
	printf("%x\n",a^y);

	//最低有效字节设置成1，其他字节不变
	int z = 0xFF;
	printf("%x\n",a | z);

	exit(0);
}


