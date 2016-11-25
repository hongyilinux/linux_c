#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


#define		CPS			10
#define		BUFSIZE		CPS
static int loop = 0;
static void  alrm_handler(int s)
{
	loop = 1;
	alarm(1);
}
int main(int argc,char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage..\n");
		exit(1);
	}
	int sfd,dfd;
	dfd = 1;
	char buf[BUFSIZE];	
	int len,ret;
	int  pos;	
	do
	{
		sfd = open(argv[1],O_RDONLY);
		if(sfd < 0)
		{
			if(errno != EINTR)
			{
				perror("open()");
				exit(1);
			}
		}
	}while(sfd < 0);

	signal(SIGALRM,alrm_handler);
	alarm(1);

	
	while(1)
	{
		while(!loop);
		
		loop = 0;
		len = read(sfd,buf,BUFSIZE);
		if(len < 0)
		{
			perror("read()");
			exit(1);
		}
		if(len == 0)
			break;
		pos = 0;
		while(len)
		{
			ret = write(dfd,buf+pos,len);
			while(ret < 0 )
			{
				if(errno == EINTR)
					continue;
				perror("write()");
				break;
			}
			len -= ret;
			pos += ret;
		}		

	}
	
	close(sfd);

	exit(0);
}



