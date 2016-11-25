#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define		CPS			10
#define		BUFSIZE		CPS
#define		BURST		100
static int token = 0;

static void alrm_handler(int s)
{
	alarm(1);
	token++;
	if(token > BURST)
		token = BURST;
}

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("Usage...\n");
		exit(1);
	}
	signal(SIGALRM,alrm_handler);
	alarm(1);	
	
	int sfd,dfd;
	dfd = 1;
	char buf[BUFSIZE];		
	int len,ret,pos;
	
	while((sfd = open(argv[1],O_RDONLY)) < 0)
	{
		if(sfd < 0)
		{
			if(errno == EINTR)
				continue;
			perror("open()");
			exit(1);
		}
	}
	
	while(1)
	{
		while (token <= 0)
			pause();
		token--;
		do
		{
			len = read(sfd,buf,BUFSIZE);
			if(errno != EINTR)
			{
				perror("read()");
				exit(1);
			}
			
		}while(len < 0);
		
		if(len == 0)
			break;

		pos = 0;
		
		while(len)
		{
			while((ret = write(dfd,buf+pos,len)) < 0) 
			{
				if(errno != EINTR)
				{
					perror("Write");
					exit(1);
				}
			}
			len -=ret;
			pos +=ret;
						
		}
			

	}

	close(sfd);
	exit(0);
}



