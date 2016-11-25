#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mytbf.h"

#define		CPS			10
#define		BURST		50
#define		BUFSIZE		1024


int main(int argc ,char **argv)
{
	int len,ret,pos;
	int sfd;
	int size;
	mytbf_t *job;
	char buf[BUFSIZE];

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	job = mytbf_init(CPS,BURST);
	if(job == NULL)
	{
		fprintf(stderr,"mytbf_init failed\n");
		exit(1);
	}

	while((sfd = open(argv[1],O_RDONLY)) < 0)
	{
		if(errno == EINTR)
			continue;
		perror("open");
		exit(1);
	}

	while(1)
	{
		size = mytbf_fetchtoken(job,BUFSIZE);
		if(size < 0)
		{
			fprintf(stderr,"mytbf_fetchtoken failed %s\n",strerror(-size));
			exit(1);
		}
		while((len = read(sfd,buf,size)) < 0)
		{
			if(errno == EINTR)
				continue;
			perror("read");
			exit(1);
		}

		if(size -len > 0)
			mytbf_returntoken(job,size-len);

		if(len == 0)
			break;
		
		pos = 0;
		while(len > 0)
		{
			while((ret = write(1,buf+pos,len)) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("write");
				exit(1);
			}

			pos += ret;
			len -= ret;

		}

	}
	
	mytbf_destroy(job);
	close(sfd);
	exit(0);
}


