#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	alarm(10);
	/*
	 *	如果程序中有多个alarm，
	 *	pause（）：可以防止忙等
	 *	多任务的计时器如何做？
	 *	注意没有pause的情况下，看cpu的使用率，如何避免忙等。
	 */

	while(1)
	{

	}
	
	exit(0);	
}



