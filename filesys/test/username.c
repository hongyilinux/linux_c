#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
int main(int argc,char **argv)
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	struct passwd *pwd_line;
	pwd_line = getpwuid(atoi(argv[1]));  //take note
	
	if(pwd_line == NULL)
	{
		perror("getpwuid()");
		exit(1);
	}
	
	puts(pwd_line->pw_name);
	
	exit(0);

}





