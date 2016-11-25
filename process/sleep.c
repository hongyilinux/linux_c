#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *	fork()
 *	wait()
 *	execl()
 *	以上三个函数构成linux系统的基本框
 */

int main(int argc ,char **argv)
{

	puts("Begin");
	
	fflush(NULL);
	
	pid_t pid ;
		
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if(pid == 0)
	{
		execl("/bin/sleep","sleep","100",NULL); //注意argv[1]
		//execl("/bin/sleep","httpd","100",NULL); //注意argv[1]
		
		perror("execl()");
		exit(1);
	}
	
	wait(NULL);
	puts("End");

	exit(0);	

}



