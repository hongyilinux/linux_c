#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

/*		1、注意loop的标志的使用
 *		2、注意signal（）和alarm（）函数的顺序
 *		3、注意while（1）循环下面的循环判断
 *		4、信号漏桶的实现 
 */

#define		CPS			10
#define		BUFSIZE		CPS

static int loop = 1;

static void alrm_handler(int s)
{
	alarm(1);
	loop = 0;
}

int main(int argc ,char **argv)
{

	int sfd;

	int ret,res,pos;

	char buf[BUFSIZE];

	signal(SIGALRM,alrm_handler);
	alarm(1);

	while((sfd = open(argv[1],O_RDONLY)) < 0)
	{
		if(errno == EINTR)
			continue;
		perror("open()");
		exit(1);
	}
	
	while(1)
	{
		while(loop == 1)
			pause();
		loop = 1;

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
				perror("write()");
				exit(1);
			}
			pos += res;
			ret -= res;
		}
	}

	close(sfd);

	exit(0);
}



