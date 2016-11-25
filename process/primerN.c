#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
 * 进程分配的方法：
 *	交叉分配方法
 *	分块
 *	池:会涉及竞争，需要原子操作解决
 */

#define LEFT		30000000

#define RIGHT		30000200

#define	N			3


int main(int argc ,char ** argv)
{
	int i,j,mark,n;
	pid_t pid ;

	
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
			for(i = LEFT+n ; i <= RIGHT ; i+=N)
			{


					mark = 1;
					for(j=2 ; j< i/2 ;j++)
					{
							if(i%j == 0)
									mark = 0;
					}
					if(mark)
					{
							printf("[%d:]%d is a primer \n",n,i);
					}

			}
			exit(0);
		}	
	}
	for(n = 0 ; n < N ; n++)
	{
		wait(NULL); //回收资源
	}
	exit(0);
}


