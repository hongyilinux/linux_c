#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#define		MEMSIZE			1024

/*	
 *	父进程先mmap一块内存空间，后fork，子进程写字符串，父进程读字符串
 *
 *
 *
 */

int main()
{
	char *ptr;

	pid_t pid;

	//匿名映射相当于malloc，fd参数必须为-1
	ptr = mmap(NULL,MEMSIZE,PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS,-1,0);

	if(ptr == MAP_FAILED)
	{
		perror("mmap()");
		exit(1);
	}

	pid = fork();

	if(pid < 0)
	{
		munmap(ptr,MEMSIZE);
		perror("fork");
		exit(1);
	}

	if(pid == 0)				//child wirte
	{
		//	ptr = "hello world";		//这样写是错误的
		strcpy(ptr,"Hello World");
		munmap(ptr,MEMSIZE); 	//子进程写完后，可以解除映射
		exit(0);
	}else						//parent read
	{
		wait(NULL);				//确保子进程写完
		puts(ptr);
		munmap(ptr,MEMSIZE);
	}

	exit(0);
}



