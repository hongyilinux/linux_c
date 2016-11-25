#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

static void cleanup_func (void *ptr)
{
	puts(ptr);
}

static void *func(void *p)
{
	puts("thread is working");

	pthread_cleanup_push(cleanup_func,"cleanup1");
	pthread_cleanup_push(cleanup_func,"cleanup2");
	pthread_cleanup_push(cleanup_func,"cleanup3");

	puts("push finished");

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	pthread_exit(NULL);
}

int main()
{
	
	puts("Begine");
	pthread_t tid;
	int ret;
	ret = pthread_create(&tid,NULL,func,NULL);

	if(ret)
	{
		fprintf(stderr,"%s",strerror(ret));
		exit(1);
	}

	pthread_join(tid,NULL);
	
	puts("End");

	exit(0);	
}


