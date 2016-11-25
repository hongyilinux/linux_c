#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include "mytbf.h"
#define		BURST		100
#define 	CPS				10
#define		BUFSIZE			1024

int main(int argc ,char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}


	char buf[BUFSIZE];
	
	int sfd,dfd = 1;

	int len,ret,pos;

	mytbf_t *tbf;

	tbf = mytbf_init(CPS,BURST);

	if(tbf == NULL)
	{
		fprintf(stderr,"Init failure";
		exit(1);
	}

	sfd = open(argv[1],O_RDONLY);
	if(sfd < 0)
	{
		perror("open");
		exit(1);
	}
	
	while(1)
	{
		
		size = mytbf_fetchtoken(tbf,BUFSIZ);

		while((len = read(sfd,buf,size)) < 0 )
		{
			if(errno == EINTR)
				continue;
		}
		if(len == 0)
		{
			break;
		}
		
		if(size - len > 0)
			mytbf_returntoken();

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
	mytbf_destroy(*tbf);	
	close(sfd);
	
}



