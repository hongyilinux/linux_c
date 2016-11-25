#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct 
{
	int d1;
	int d2;
}Arg;

static void * thr_func(void * arg)
{
	Arg *v = arg;

	return (void *)(v->d1+v->d2);
}

int main()
{
	pthread_t tid;

	int err;
	
	//结构体的一种初始化模式
	Arg arg = {20,50};

	err = pthread_create(&tid,NULL,thr_func,(void *)&arg);

	if(err)
	{
		fprintf(stderr,"pthread_create err %s\n",strerror(err));
		exit(1);
	}

	//pthread_join的用法，接收返回值
	
	/*	方法一：知道thr_func线程返回的int类型，用int result
	 *	需要注意的：void(**)的类型和&result是一样的，都是指针变量
	int result;

	pthread_join(tid,&result);

	printf("result is %d\n",result);
	*/

	/*
	 * 方法二：把指针当变量用，需要用心体会

	int *result ;
	
	//二级指针传参
	pthread_join(tid,&result);
	
	//把地址当成int来用
	printf("result is %d\n",(int)result);
	*/
	exit(0);
}

