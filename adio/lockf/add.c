#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 *	20个进程，写文件
 *
 *
 */

#define		FNAME			"/tmp/out"
#define		LINEBUF			1024
#define		PROCNUM			20

static void func_add()
{
	FILE *fp;
	char buf[LINEBUF];
	int fd;
	fp = fopen(FNAME,"r+");
	if(fp == NULL)
	{
		perror("fopen");
		exit(1);
	}
	fd = fileno(fp);

	//注意参数0的使用,0 表示文件有多长，锁多长
	lockf(fd,F_LOCK,0);
	fgets(buf,LINEBUF,fp);
	//重定位文件位置指针
	fseek(fp,0,SEEK_SET);
	fprintf(fp,"%d\n",atoi(buf)+1);
	fflush(fp);
	lockf(fd,F_ULOCK,0);
	fclose(fp);
}

int main()
{
	int i;
	pid_t pid;

	for(i = 0 ; i < PROCNUM ; i++)
	{
		pid = fork();

		if(pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if(pid == 0)
		{
			//child
			func_add();
			exit(0);
		}
	}
	
	for(i = 0 ; i < PROCNUM ; i++)
		wait(NULL);

	exit(0);
}

