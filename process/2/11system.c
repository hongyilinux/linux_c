#include <stdio.h>
#include <stdlib.h>
/*	功能：system函数
 *	system 相当于 fork + wait + exec
 */

int main()
{
	system("date +%s > /tmp/out");


	exit(0);
}



