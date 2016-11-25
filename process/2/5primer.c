#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define LEFT	30000000
#define RIGHT	30000200
#define N 		3
/*
 *	体会
 *	分块分配
 *	交叉分配的方法
 *	池类算法。
 *
 * 本例采用交叉分配算法
 */
int main()
{
	int i,j,mark,n;
	pid_t pid;	
	for(n = 0 ; n < N ; n++)
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork()");
			exit(1);
		}
		if(pid == 0)
		{
			for(i = LEFT+n; i <= RIGHT ; i+=N)  //此处需要注意
			{
				mark = 1;
				for(j = 2; j < i/2 ; j++)
				{
					if(i % j == 0)
					{
						mark = 0;
						break;
					}
				}

				if(mark)
				{
					printf("[%d:]%d is a primer \n",getpid(),i);
				}	
			}
			exit(0);		//注意这个exit非常重要的
		}	
	}

	for(n = 0 ; n < N ;n++)   //父进程等待回收资源，回收201个子进程
	{	
		wait(NULL);
	}
	exit(0);
}



