#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define		LEFT		30000000
#define		RIGHT		30000200
#define		THRNUM		4

/*	num = 0，main线程可以下发任务
 *	num > 0 ,main线程任务下发完毕，运算线程可以抢任务，进行计算
 *	num = -1 ,所有任务下发完成，main线程等待计算线程完成后退出，回收资源
 */

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

static void * thr_func(void *p)
{
	int i,j,mark;

	while(1)
	{
		pthread_mutex_lock(&mut_num);
		while(num == 0)
		{
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}
		if(num == -1)
		{
			pthread_mutex_unlock(&mut_num);
			break;
		}
		
		i = num;
		num = 0;
		pthread_mutex_unlock(&mut_num);
		
		mark = 0;
		for(j = 2 ; j < i/2 ; j++)
		{
			if(i % j == 0)
			{
				mark = 1 ;
				break;
			}
		}
		if(mark == 0)
		{
			printf("[%d] %d is primer\n",(int)p,i);
		}

	}

	pthread_exit(NULL);
}

int main()
{
	int i,err;
	pthread_t tid[THRNUM];

	for(i = 0 ; i < THRNUM ; i++)
	{
		err = pthread_create(tid+i,NULL,thr_func,(void *)i);
		if(err)
		{
			fprintf(stderr,"pthread_create():%s\n",strerror(err));
			exit(1);
		}
	}

	//main线程分发任务
	for(i = LEFT ; i <= RIGHT ; i++)
	{
		pthread_mutex_lock(&mut_num);
		while(num != 0)
		{
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}
		num = i;
		pthread_mutex_unlock(&mut_num);
	}


	//main线程任务下发完毕，修改num = -1
	pthread_mutex_lock(&mut_num);
	//while(num != -1) 是错误写法
	while(num != 0)
	{
		pthread_mutex_unlock(&mut_num);
		sched_yield();
		pthread_mutex_lock(&mut_num);
	}
	num = -1;
	pthread_mutex_unlock(&mut_num);

	//main线程任务下发结束，main线程准备回收资源
	for(i = 0 ; i < THRNUM ; i++)
	{
		pthread_join(tid[i],NULL);
	}
	
	pthread_mutex_destroy(&mut_num);

	exit(0);
}



