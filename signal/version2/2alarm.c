#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/*	1、多个alarm函数使用的时候会出错，以最后一个alarm为准
 *	2、while后面如果没有pause函数，会导致忙等，占满整个CPU
 *
 *
 */

int main()
{
	alarm(5);
	alarm(1);
	alarm(10);

	while(1)
		pause();


	exit(0);
}



