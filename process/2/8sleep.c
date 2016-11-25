#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*	功能：实现sleep100 的功能
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
		execl("/bin/sleep","httpd","100",NULL);	//注意argv[0] 实现隐藏	
		perror("execl()");
		exit(1);
	}
	wait(NULL);
	puts("End");
	
	exit(0);

}




