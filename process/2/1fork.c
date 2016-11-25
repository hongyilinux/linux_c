#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*	用到的函数：getpid ，fork
 *	注意fork的返回值
 *	注意fork的含义，fork之前如果没有fflush(NULL);可能出现异常
 *	注意fork之前如果没有fflush(NULL)的话，./1fork > /tmp/out 
 *	注意，父子进程及shell进程谁先运行，谁后运行时不知道，不确定的
 */
int main()
{

	pid_t pid;
	
	printf("[%d:] Begin\n",getpid());

	pid = fork();
	
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}else if(pid == 0) //child
	{
		printf("[%d:] child is working \n",getpid());	
	}else		//parent
	{
		printf("[%d:] parent is working \n",getpid());	
	}



	printf("[%d:] End\n",getpid());


	exit(0);
}



