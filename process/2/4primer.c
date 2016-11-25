#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define LEFT	30000000
#define RIGHT	30000200
/*
 *	着重体会wait的含义及意义
 *
 *
 */
int main()
{
	int i,j,mark;
	pid_t pid;	

	for(i = LEFT; i < RIGHT ; i++)
	{
			pid = fork();
			if(pid < 0)
			{
				perror("fork()");
				exit(1);
			}
			if(pid == 0)
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
							printf("%d is a primer \n",i);
					}
					exit(0);		//注意这个exit非常重要的
			}
	}

	for(i = LEFT ; i<= RIGHT ; i++)   //父进程等待回收资源，回收201个子进程
	{
			wait(NULL);
	}

	exit(0);
}



