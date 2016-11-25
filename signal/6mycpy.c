#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define		BUFSIZE		1024

int main(int argc,char **argv)
{
	if(argc < 3)
	{
		fprintf(stderr,"Usage..\n");
		exit(1);
	}
	int sfd,dfd;
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

	do
	{
		dfd = open(argv[2],O_RDWR|O_TRUNC|O_CREAT,0600);
		if(dfd <0)
		{
			if(errno != EINTR)
			{
				perror("open");
				exit(1);
			}
		}
	}while(dfd <0);
	
	while(1)
	{
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
	


	exit(0);
}



