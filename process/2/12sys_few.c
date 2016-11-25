#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*	功能：system函数
 *	system 相当于 fork + wait + exec
 */

int main()
{
//	system("date +%s > /tmp/out");

	pid_t pid;
	
	pid = fork();
	if(pid < 0)
	{
		perror("fork()");
		exit(1);
	}
	
	if(pid == 0)
	{
		execl("/bin/sh","sh","-c","date +%s",NULL);	
		perror("execl()");
		exit(1);
	}

	wait(NULL);

	exit(0);
}



