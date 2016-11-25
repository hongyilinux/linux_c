#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



#define FNAME	"/tmp/out"
//程序还是不完善


int main ()
{

	int fd;
	
//	close(1);
	
	fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0600);
	if(fd < 0)
	{
		perror("open()");
		exit(1);
	}

	dup2(1,fd);
	/*
 	*close(1);
 	*dup(fd);
 	*
 	*/
	
	if(fd != 1)
	{
		close(fd);
	}
/************************************/	
	puts("hello");

}

