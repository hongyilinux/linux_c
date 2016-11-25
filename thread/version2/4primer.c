#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define		LEFT			30000000
#define		RIGHT			30000200
#define		THRNUM			(RIGHT-LEFT +1)

struct thr_arg_st
{
	int num;
};

static void * thr_func(void *p)
{
	//强制类型转换,取到带计算的数
	int n = ((struct thr_arg_st *)p)->num;

	//此处可以直接free(p).但常规做法需要malloc和free在同一函数模块中
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
		printf("%d is primer\n",n);

	//把p返回值pthread_join中，由pthread_join 后面的语句freE
	pthread_exit(p);

}


int main()
{
	int i,err;
	pthread_t tid[THRNUM];

	//线程函数的参数，用结构体表示
	struct thr_arg_st *arg_num;

	void *ptr;

	for(i = LEFT ; i <= RIGHT ; i++)
	{
		//参数需要malloc内存，放置竞争
		arg_num = malloc(sizeof(*arg_num));
		if(arg_num == NULL)
		{
			perror("malloc");
			exit(1);
		}
		arg_num->num = i;
		err = pthread_create(tid+i-LEFT , NULL, thr_func,arg_num);
		
		if(err)
		{
			fprintf(stderr,"pthread_create failed %s\n",strerror(err));
			exit(1);
		}

	}
	
	for(i = 0 ; i < THRNUM ; i++)
	{
		//pthread_join 用ptr回填pthread_exit返回的参数地址
		pthread_join(tid[i],&ptr);
		free(ptr);
	}

	exit(0);
}


