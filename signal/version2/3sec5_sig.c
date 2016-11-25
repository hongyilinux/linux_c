#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
 *	signal控制时间比time要精准一些
 *	gcc 3sec5_sig.c -O1 
 *
 *
 */

static volatile int loop = 1;

static void alrm_handler(int s)
{
	loop = 0;
}

int main()
{
	int64_t count = 0;

	signal(SIGALRM,alrm_handler);
	alarm(5);

	while(loop)
		count++;

	printf("%lld\n",count);
}


