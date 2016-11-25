#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


#define		LEFT		30000000
#define		RIGHT		30000200
#define		THRNUM		(RIGHT-LEFT+1)

static void * thr_func(void *p)
{
	//此处强制类型转化可造成编译警告
	int n = (int) p;

	int i,mark;
	
	mark = 1;
	for(i = 2 ; i < n/2 ; i++)
	{
		if(n%i == 0)
		{
			mark = 0; 
			break;
		}
	}
	if(mark)
	{
		printf("%d is primer\n",n);
	}

	pthread_exit(NULL);
}

int main()
{
	int i;
	int err;
	pthread_t tid[THRNUM];

	for(i = LEFT ; i<= RIGHT ; i++)
	{
		//注意此处的强制类型转换，会导致编译警告
		err = pthread_create(tid+i-LEFT ,NULL,thr_func,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_create faild %s\n",strerror(err));
			exit(1);
		}

	}

	//谁创建，谁回收，创建多少，回收多少
	for(i = 0 ; i < THRNUM ; i++)
	{
		pthread_join(tid[i],NULL);
	}

	exit(0);
}






