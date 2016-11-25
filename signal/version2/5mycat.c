#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define		BUFSIZE			1024

int main(int argc ,char **argv)
{
	int ret,res,pos;

	int sfd;

	char buf[BUFSIZE];

	while((sfd = open(argv[1],O_RDONLY)) < 0)
	{
		if(errno == EINTR)
			continue;
		perror("open()");
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
			while((res = write(1,buf+pos,ret)) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write");
				exit(1);
			}
			pos += res;
			ret -= res;
		}
	}


	close(sfd);
	exit(0);
}



