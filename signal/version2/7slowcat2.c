#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define		BURST		1024
#define		CPS			10
#define		BUFSIZE		CPS

static volatile int token = 0;

static void alrm_handler(int s)
{
	alarm(1);
	token++;

	if(token >= BURST)
		token = BURST;
}

int main(int argc ,char **argv)
{
	int sfd;
	int ret,res,pos;

	char buf[BUFSIZE];

	signal(SIGALRM,alrm_handler);
	alarm(1);
	
	while((sfd = open(argv[1],O_RDONLY)) < 0)
	{
		if(errno == EINTR)
			continue;
		perror("open");
		exit(1);
	}

	while(1)
	{
		while(token <= 0)
			pause();
		token--;

		while((res = read(sfd,buf,BUFSIZE)) < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read()");
			exit(1);
		}
		if(res == 0)
			break;
		
		pos = 0;
		while(res > 0)
		{
			while((ret = write(1,buf+pos,res)) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write");
				exit(1);
			}
			pos += ret;
			res -= ret;
		}
	}

	close(sfd);
	exit(0);
}




