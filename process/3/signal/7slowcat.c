#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define 	CPS				10
#define		BUFSIZE			CPS

static volatile int loop =  0;

static void alrm_handler(int s)
{
	alarm(1);
	loop = 1;
}

int main(int argc ,char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}


	signal(SIGALRM,alrm_handler);
	alarm(1);

	char buf[BUFSIZE];
	
	int sfd,dfd = 1;

	int len,ret,pos;

	sfd = open(argv[1],O_RDONLY);
	if(sfd < 0)
	{
		perror("open");
		exit(1);
	}
	
	while(1)
	{
		//此处产生了一个忙等
		/*
		while(!loop)
			;
		*/
		//这样就不会产生忙等待了
		while (!loop)
			pause();
		loop = 0 ;
		len = read(sfd,buf,BUFSIZE);
		if(len < 0)
		{
			perror("read");
			break;
		}
		if(len == 0)
		{
			break;
		}
		/*典型的错误写法
		ret = write(dfd,buf,len);
		if(ret < 0)
		{
			perror("write");
			exit;
		}
		*/
		pos = 0;
		while(len > 0)
		{
			ret = write(dfd,buf+pos,len);
			if(ret < 0)
			{
				perror("write");
				exit(1);
			}
			pos += ret;
			len -= ret;
		}


	}	
	


	close(sfd);
	
}



