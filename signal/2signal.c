#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
/*
 *	
 *	体会signal函数的用法，打印*号
 *	如果没有信号，ctrl+c 会打断程序的执行。就是直接杀死了进程 ctr+C = SIGINT， ctr+/ = SIGQUIT
 */

static void int_handler (int s)
{
	write(1,"!",1);
}


int main()
{
	int i;
	
	//signal(SIGINT,SIG_IGN);	

	signal(SIGINT,int_handler);   //信号动作的执行条件：1、信号来了，2、程序没有结束

	for(i = 0 ; i < 10 ; i++)
	{
		write(1,"*",1);
		sleep(1);
	}

	exit(0);
}



