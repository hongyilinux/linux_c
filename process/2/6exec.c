#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *打印时间戳。data +%s
 *
 */

int main()
{
	puts("Begin");
	fflush(NULL);
	execl("/bin/date","date","+%s",NULL);
	perror("exec()");   //注意此处的巧妙用法。此处不用使用if
	exit(1);
	puts("End");	


	exit(0);
}



