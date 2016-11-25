#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static sigint_is_occ = 0;

static void sig_handler(int signo)
{
	printf("sigint is occured %d\n",signo);
	sigint_is_occ = 1;
}



int main()
{
	if(signal(SIGINT,sig_handler) == SIG_ERR)
	{
		perror("signal error");
		exit(1);
	}
	
	puts("begin");

	while(sigint_is_occ == 0)
	{
		//如果ctrl+c信号在sleep（5）这个时间窗口发出，那么while后面的代码将执行不到
		sleep(5);
		pause();
	}

	puts("end");



}


