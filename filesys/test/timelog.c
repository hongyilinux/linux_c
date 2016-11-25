#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PATH	"/tmp/timlog"
#define BUFSIZE		1024		//数组的长度应该要足够长，才能统计行数	

int main()
{
	
	FILE *fp;
	time_t time_now;
	int i=0;
	struct tm *gm_st;	
	char buf[BUFSIZE];
	fp = fopen(PATH,"a+");
	if(fp == NULL)
	{
		fprintf(stderr,"fopen error\n");
		exit(1);
	}	
	while(fgets(buf,BUFSIZE,fp) != NULL)
		i++;	
	

	while(1)
	{
		time_now = time(NULL);  //注意   time(&time_now)也是对的

		gm_st = gmtime(&time_now);
	
		fprintf(fp,"%d %d-%d-%d %d:%d:%d \n",i++,gm_st->tm_year+1900,gm_st->tm_mon+1,gm_st->tm_mday,gm_st->tm_hour,gm_st->tm_min,gm_st->tm_sec);
		fflush(fp);
		sleep(1);
	}
	
	exit(0);

}
















