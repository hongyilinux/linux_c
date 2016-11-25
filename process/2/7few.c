#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*	功能：组合实现fork exec wait 实现进程的合作
 *
 *
 */
int main()
{
	pid_t pid;
	puts("Begin");
	
	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(0);
	}
	if(pid == 0)
	{
		execl("/bin/date","date","+%s",NULL);	
		perror("execl()");
		exit(1);
	}
	wait(NULL);
	puts("End");
	
	exit(0);

}




