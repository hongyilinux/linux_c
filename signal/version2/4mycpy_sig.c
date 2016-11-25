#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define		BUFSIZE			1024


int main(int argc ,char **argv)
{
	int sfd,dfd;
	int ret,res,pos;
	char buf[BUFSIZE];

	if(argc < 3)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	while((sfd = open(argv[1],O_RDONLY)) < 0)
	{
		if(errno == EINTR)
			continue;
		perror("open");
		exit(1);
	}

	while((dfd = open(argv[2],O_RDWR|O_TRUNC|O_CREAT,0600)) < 0)
	{
		if(errno == EINTR)
			continue;
		close(sfd);
		perror("open");
		exit(1);
	}

	while(1)
	{
		while((ret = read(sfd,buf,BUFSIZE)) < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read()");
			exit(1);
		}
		if(ret == 0)
			break;
		
		pos = 0;
		while(ret > 0)
		{
			while((res = write(dfd,buf+pos,ret)) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write");
				exit(1);
			}
			pos += res;
			ret -= ret;
		}
	}

	close(sfd);
	close(dfd);
	exit(0);
}



