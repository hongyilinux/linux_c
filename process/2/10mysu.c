#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
/*
 *	功能：实现类似sudo的功能
 *	./mysu 0 cat /etc/shadow
 */
int main(int argc,char **argv)
{
	pid_t pid;
	if(argc < 3)
	{
		fprintf(stderr,"usage...\n");
		exit(1);		
	}	
	
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}	
	if(pid == 0)
	{
		setuid(atoi(argv[1]));   //字符串转换
		execvp(argv[2],argv+2);
		perror("execvp");
		exit(1);
	}

	wait(NULL);

	exit(0);
}



