#include <stdio.h>
#include <stdlib.h>

/*
 *输出所有的环境变量
 *
 */

extern	char **environ;

int main()
{
 	int i;
	
	for(i = 0 ; environ[i] != NULL; i++)
		puts(environ[i]);

	exit(0);
}




