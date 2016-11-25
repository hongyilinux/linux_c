#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "mytbf.h"

#define		CPS			10
#define		BUFSIZE		1024
#define		BURST		50

int main(int argc ,char **argv)
{
	int sfd;
	int len ,ret,pos;
	char buf[BUFSIZE];
	int size;
	mytbf_t *me;

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	me = mytbf_init(CPS,BURST);
	if(me == NULL)
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
		size = mytbf_fetchtoken(me,BUFSIZE);
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
		if(len == 0)
			break;
	
		if(size - len > 0)
		{
			mytbf_returntoken(me,size -len );
		}
		
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
	
	close(sfd);
	mytbf_destroy(me);
	exit(0);
}


