#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * 实现打印时间戳
 * data +%s
 * 
 * 充分理解 replace
 *
 * 输出重定向到：/tmp/out会发现 "Begin"是没有了的
 * 使用exec 需要fflush（NULL）
 * 使用exec 需要刷新缓冲区
 */


int main ()
{

	puts("Begin");	
	
	if(fflush(NULL) == EOF)
		perror("fflush()");
	execl("/bin/date","+%s",NULL);
	
	perror("execl()");
	exit(1);

	puts("End");// "End" 是不能打印出来的。
	exit(0);
}



