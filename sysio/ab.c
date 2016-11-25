#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/**
 *看系统调用和标准io的区别。
    strace 查看
 *
 * */



int main ()
{
	putchar('a');
	write(1,"b",1);

	putchar('a');
	write(1,"b",1);

	putchar('a');
	write(1,"b",1);
	
	exit(0);
}



