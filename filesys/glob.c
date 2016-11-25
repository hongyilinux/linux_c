#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>

//#define PAT		"/etc/*.conf"
#define PAT 	"/etc/*"             //找/etc/目录下的所有文件。
static int errfunc__(const char *errpath,int eerrno)
{
	puts(errpath);
	fprintf(stderr,"ERROR MESG %s \n",strerror(eerrno));
	return 0;
}

int main (int argc,char **argv)
{
	int err, i;
	glob_t globres;
	err = glob(PAT,0,errfunc__,&globres);
	if(err)
	{
		printf("Error code is %d \n",err);
		exit(1);
	}
	
	for(i = 0 ; i < globres.gl_pathc;i++)
	{
		puts(globres.gl_pathv[i]);
	}
	globfree(&globres);
	exit(0);

}





