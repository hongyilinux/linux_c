#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *功能：求出100天以后，是几月几号
 *
 */

#define	TIMESTRSIZE		1024

int main()
{

	time_t stamp;
	struct tm  *tm;
	stamp = time(NULL);
	
	char timestr[TIMESTRSIZE];


	tm = localtime(&stamp);

	strftime(timestr,TIMESTRSIZE,"NOW:%Y-%m-%d",tm);	
	
	puts(timestr);	

	
	tm->tm_mday += 100;

	mktime(tm);  //功能会自动进位，当天数多看自动向月份进位，月份满了自动向年份进位
	
	strftime(timestr,TIMESTRSIZE,"100 days later is %Y-%m-%d",tm);
	
	puts(timestr);

	exit(0);
}










