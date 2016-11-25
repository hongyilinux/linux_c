#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define  BUFSIZE 		1024

int main (int argc,char **argv)
{
	
	if(argc < 3)
	{
		fprintf(stderr,"Usage...\n");
		exit(0);
	}
	
	int sfd,dfd;
	char buf[BUFSIZE];
	int len,ret,pos;
	sfd = open(argv[1],O_RDONLY);
	if(sfd < 0)
	{
		perror("open()");
		exit(1);
	}
	dfd = open(argv[2],O_RDWR|O_CREAT,O_TRUNC,0600); //无则创建，有则清空，若有O_CREAT，需用三参open

	if(dfd <0)
	{
		close(sfd);
		perror("open()");
		exit(1);
	}
	
	while(1)
	{
		len = read(sfd,buf,BUFSIZE);
		if(len <0)   //读取出错
		{
			perror("read()");
			exit(0);
		}
		if(len == 0)
			break;
		pos = 0;
		while(len)
		{
			ret = write(dfd,buf+pos,len);
			if(ret < 0)
			{
				perror("write()");
				break;
			}
			len -= ret;
			pos += ret;	
		}
				
	}	
	
	
	
	close(dfd);
	close(sfd);
	
	
	exit(0);
}









