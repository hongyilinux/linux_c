#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void * func(void *ptr)
{
	puts("Thread is working");
	//return NULL;			//函数的返回
	pthread_exit(NULL);    //线程终止的方法
}

int main()
{
	puts("Begin");
	
	pthread_t tid;
	int err;

	err = pthread_create(&tid,NULL,func,NULL);
	

	if(err)
	{
		fprintf(stderr,"pthread create %s\n",strerror(err));
		exit(1);
	}

	pthread_join(tid,NULL);

	puts("End");
	

	exit(0);	
}


