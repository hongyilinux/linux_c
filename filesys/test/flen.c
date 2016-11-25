#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static off_t flen(const char *fname)   //禁止外部扩展
{
	struct stat statres;
	
	if(stat(fname,&statres) < 0)
	{
		perror("stat()");
		exit(1);
	}
	return statres.st_size;
				
}

int main(int argc,char **argv)
{

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
//		printf("%lld \n",(long long)flen(argv[1]));  //如果不修改makefile 需要(long long)强转
		printf("%lld \n",flen(argv[1]));



	exit(0);
}










