#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	time_t end;
	int64_t count = 0;
	end = time(NULL);

	while(time(NULL) <= end+5)
		count++;
	
	printf("the count is %lld \n",count);

	exit(0);
}


