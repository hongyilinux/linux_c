#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/*
 *	体会用户调用的过程中产生信号
 *
 */

static void sig_handler(int signo)
{
	if(signo == SIGTSTP)
	{
		printf("SIGTSTP occured\n");
	}
}

static void user_call_func(void)
{
	int i;

	printf("user call begine\n");
	//模拟产生时间窗口，此时发生系统调用	
	for(i = 0 ; i < 10 ; i++)
	{
		printf("%d\n",i);
		sleep(1);
	}	

	printf("user call end\n");
}

int main()
{
	if(signal(SIGTSTP,sig_handler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}

	printf("main begin\n");

	user_call_func();

	printf("main end\n");


	exit(0);
}



