#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* 1、调用read 系统调用，程序阻塞，会一直等待用户输入
 * 2、用户输入到一半，ctrl+z ，这时候会重新启动read系统调用
 * 3、通过光标的位置就可以发现
 * 4、就是说，当慢系统调用的过程中产生信号，内核会重启系统调用
 */

#define		BUFSIZE			1024

static void sig_handler(int signo)
{
	printf("SIGTSTP is occured \n");
}

int main()
{
	char buf[BUFSIZE];
	ssize_t size;

	if(signal(SIGTSTP,sig_handler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	printf("process begine and waitng a SIGTSTP\n");

	size = read(STDIN_FILENO,buf,BUFSIZE);
	if(size < 0)
	{
		perror("read error");
	}
	printf("read finished\n");
	
	if(write(STDOUT_FILENO,buf,size) != size)
	{
		perror("write error");
	}
	printf("write finished\n");

	exit(0);
}


