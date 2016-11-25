#include <stdio.h>
#include <stdlib.h>

int main()
{
	int x = 53191;
	short sx = (short)x;
	
	short sy = -1;

	int y = sx;

	printf("x = %d \n sx=%d\n,y=%d\n",x,sx,y);
	
	int a = sy;

	printf("%d\n",x);

	exit(0);
}


