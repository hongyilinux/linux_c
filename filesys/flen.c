#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int flen(const char *fname)
{
	struct stat statres;
	if(stat(fname,&statres) < 0 )
	{
		perror("stat()");
		exit(1);
	}
	return statres.st_size;   //注意st_size 是off_t类型


}



int main (int argc ,char **argv)
{

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	printf("Total size is %lld \n",flen(argv[1]));
//	printf("Total size is %lld \n",(long long)flen(argv[1]));
	exit(0);
}












