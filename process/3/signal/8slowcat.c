#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define		BURST		100

#define 	CPS				10
#define		BUFSIZE			CPS

static volatile int  token =  0;

static void alrm_handler(int s)
{
	alarm(1);
	token++;
	if(token > BURST)
		token = BURST;
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
		//这样就不会产生忙等待了
		while (token <= 0)
			pause();
		token--;
		while((len = read(sfd,buf,BUFSIZE)) < 0 )
		{
			if(errno == EINTR)
				continue;
		}
		if(len == 0)
		{
			break;
		}

		pos = 0;
		while(len > 0)
		{
			while((ret = write(dfd,buf+pos,len)) < 0)
			{
				if(errno == EINTR)
					continue;
			}
			pos += ret;
			len -= ret;
		}


	}	
	
	close(sfd);
	
}



