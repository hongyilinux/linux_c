#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static int file_type(const char *fname)
{
	struct stat statres;
	if((stat(fname,&statres)<0))
	{
		perror("stat");
		exit(1);
	}
	if(S_ISREG(statres.st_mode))
		return '-';
	if(S_ISDIR(statres.st_mode))
		return 'd';
	if(S_ISCHR(statres.st_mode))
		return 'c';
	if(S_ISBLK(statres.st_mode))
		return 'b';
	if(S_ISFIFO(statres.st_mode))
		return 'p';
	if(S_ISLNK(statres.st_mode))
		return 'l';
	if(S_ISSOCK(statres.st_mode))
		return 's';
	
			
}

int main (int argc ,char **argv)
{
	
	if(argc < 2)
	{
		fprintf(stderr,"Usage....\n");
		exit(1);
	}
	
	int ch ;
	ch = file_type(argv[1]);

	if(ch == '-')
		printf("%s is %c\n",argv[1],ch);
	if(ch == 'd')
		printf("%s is %c\n",argv[1],ch);
	if(ch == 'l')
		printf("%s is %c\n",argv[1],ch);
	if(ch == 'p')
		printf("%s is %c\n",argv[1],ch);
	if(ch == 'c')
		printf("%s is %c\n",argv[1],ch);
	if(ch == 'b')

	exit(1);
}

