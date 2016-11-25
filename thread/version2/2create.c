#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
 *	1、注意pthread_exit()的位置
 *	2、pthread_cleanup_pop 和pthread_cleanup_push 是两个宏，而不是函数
 *	3、pthread_cleanup_pop 和 pthread_cleanup_push 必须成对出现 即便是pthread_cleanup_pop在pthread_exit的后面
 *
 */


static void cleanup_func(void *p)
{
	puts(p);
}

static void * thr_func(void *p)
{
	puts("Thread is working!");

	pthread_cleanup_push(cleanup_func,"aaa");
	pthread_cleanup_push(cleanup_func,"bbb");
	pthread_cleanup_push(cleanup_func,"ccc");

	//pthread_exit(NULL);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;

	int err;

	puts("Begin!");

	err = pthread_create(&tid,NULL,thr_func,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create():%s\n",strerror(err));
		exit(1);
	}

	pthread_join(tid,NULL);
	puts("End!");

	exit(0);
}


