#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#define PAT		"/etc/pass*"

int errfunc__(const char *errpath,int errno)
{
	puts(errpath);
	fprintf(stderr,"ERROR MSG:%s\n",strerror(errno));
	return 0;
}

int main(int argc,char **argv)
{
	glob_t globres;

	int err,i;

	err = glob(PAT,0,errfunc__,&globres);	
	
	if(err)
	{
		printf("Error code = %d\n",err);
	}
	
	for(i =0 ; i < globres.gl_pathc; i++)
	{
		puts(globres.gl_pathv[i]);
	}
	
	exit(0);
}











