#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void * thr_func(void *p)
{
	puts("thread is working!");
	pthread_exit(NULL);;
}

int main()
{
	pthread_t tid;
	int err;

	puts("Begin!");

	err = pthread_create(&tid,NULL,thr_func,NULL);

	if(tid < 0)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

	pthread_join(tid,NULL);

	puts("End!");

	exit(0);
}


