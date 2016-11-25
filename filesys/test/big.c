#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main (int argc , char **argv)
{
	if(argc <2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	int fd;
	fd = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0600);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
	
	lseek(fd,5LL*1024LL*1024LL*1024LL-1LL,SEEK_SET);
	//lseek(fd,(%*1024*1024*1024-1)(LL),SEEK_SET)  // wrong
	write(fd,"\0",1); //syscall
	
}
