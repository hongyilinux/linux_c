#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/*	讨论信号的可靠性 
 *	1、同类型的信号出现多次，只会处理一次
 *	2
 */

static void sig_handler(int s)
{
	if(s == SIGINT)
	{
		printf("%d catch the SIGINT\n",getpid());
		sleep(5);
		printf("process the SIGINT finished\n");
	}

	if(s == SIGTSTP)
	{
		printf("%d catch the SIGTSTP\n",getpid());
		sleep(5);
		printf("process the SIGTSTP finished\n");
	}
}

int main()
{
	//ctrl+c
	if(signal(SIGINT,sig_handler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}

	//ctr+z
	if(signal(SIGTSTP,sig_handler) == SIG_ERR)
	{
		perror("signal");
	}
	
	puts("begine");
	while(1)
		pause();
	exit(0);
}


