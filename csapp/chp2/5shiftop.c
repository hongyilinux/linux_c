#include <stdio.h>
#include <stdlib.h>

int main()
{
	int x = 0x87654321;
	

	printf("shift left 4 :%x\n",x<<128);

	printf("shift right 4: %x\n",x>>128);
	
	exit(0);
}


