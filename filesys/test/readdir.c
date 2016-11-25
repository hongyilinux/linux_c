#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define PAT		"/etc/"

/*
 *opendir
 *readir
 *closedir
 */


int main ()
{

	DIR *dir_stream;

	struct dirent *dir_res;
	
	dir_stream = opendir(PAT);
	
	if(dir_stream == NULL)
	{
		perror("opendir()");
		exit(1);
	}

	while((dir_res = readdir(dir_stream)) != NULL)
		puts(dir_res->d_name);
	
	close(dir_stream);
	
	
	exit(1);
}





