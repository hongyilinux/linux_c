#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *	注意 int64_t 的写法
 *	
 *	time()函数的误差会比较大
 * 	
 */

int main()
{
	time_t end;
	int64_t count = 0;

	end = time(NULL) + 5;

	while(time(NULL) <= end)
		count++;

	printf("%lld\n",count);

	exit(0);
}



