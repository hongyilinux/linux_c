#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include "mytbf.h"

#define		CPS			10
#define		BUFSIZE		1024
#define		BURST		100
static int token = 0;

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("Usage...\n");
		exit(1);
	}
	
	int sfd,dfd;
	dfd = 1;
	char buf[BUFSIZE];		
	int len,ret,pos,size;
	mytbf_t *tbf;		

	tbf = mytbf_init(CPS,BURST);
	if(tbf == NULL)
	{
		fprintf(stderr,"tbf_init failure\n");
		exit(1);
	}
	
	

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
		size = mytbf_fetchtoken(tbf,BUFSIZE);
		/*
		if(size < 0)
		{
			fprintf(stderr,"mytbf_fetchtoken failed %s\n",strerror(-size));
			exit(1);
		}	
		*/
		do
		{
			len = read(sfd,buf,size);
			if(errno != EINTR)
			{
				perror("read()");
				exit(1);
			}
			
		}while(len < 0);
		
		if(len == 0)
			break;

		pos = 0;
		
	//	if(size -len > 0)
	//		ret = mytbf_returntoken(tbf,size-len);	
		/*
		if(ret < 0)
		{
			fprintf(stderr,"mytbf_returntoken failed %s\n",strerror(-ret));
			exit(1);
		}
		*/		

		while(len > 0)
		{
			ret = write(dfd,buf+pos,len);
			if(ret < 0)
			{	
				perror("Write");
				exit(1);
			}
			len -=ret;
			pos +=ret;
						
		}
			

	}
	mytbf_destroy(tbf);
	close(sfd);
	exit(0);
}



