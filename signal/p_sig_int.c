#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int signo)
{
	write(1,"a",1);
	int i = 20;
	while(i > 0)
	{
		puts("sig_int handler waiting");
		i--;
		sleep(1);
	}

	signal(SIGINT,sig_handler);
}

int main()
{
	int i = 10 ;

	if(signal(SIGINT,sig_handler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	
	puts("begin");
	while(i-- > 0)
	{
		printf("main is runing\n");
		sleep(1);
	}

	puts("End");
	exit(0);
}



