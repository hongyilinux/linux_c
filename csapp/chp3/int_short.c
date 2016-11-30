#include <stdio.h>
#include <stdlib.h>

int main()
{

	int i = 32768;				//i = 32768   00 00 80 00

	short si = (short)i;		//si= -32768  80 00 
	
	int j = si;					//j = -32768  ff ff 80 00

	printf("i = %d\n",i);

	printf("j = %d\n",j);

	exit(0);
}


