#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *	一个Begine，两个End
 *
 *
 *
 *
 */



int main()
{

	pid_t pid;
	
	printf("[%d:]Begin\n",getpid()); // printf("[%d:] Begin",getpid()); 在终端上会有两次Begin
	
	fflush(NULL);  // fork 之前一定要有刷新缓存的，fork是memcpy的。
	pid = fork();
	
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	
// 在父子进程中都会判断

	if(pid == 0)			//child
	{
		printf("[%d:]child is working\n",getpid());	
	}else					//parent
	{
	//	sleep(1);  //让父进程休息，子进程先跑
		printf("[%d:] parent is working \n",getpid());	
	}

	//getchar();   //注意 ps axf 观察父子进程的关系	
	printf("[%d:]End\n",getpid());
	exit(0);
}



