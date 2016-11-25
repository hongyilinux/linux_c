#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*	time ./2primer  可以查看进程的执行时
 *	fork了201 个进程来处理，由调度程序来调度
 *	time ./3primer 可以看到程序的执行速度快很多了
 *	注意观察父子进程之间的关系
 */

#define LEFT	30000000
#define RIGHT	30000200

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


	exit(0);
}



