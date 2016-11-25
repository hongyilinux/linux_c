#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "mytbf.h"

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
	pthread_mutex_t mut_token;
};

static struct mytbf_st *job[MYTBFMAX];

pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;
pthread_once_t once_init = PTHREAD_ONCE_INIT;

static pthread_t tid;

static int inited = 0;

static int get_free_pos_unlocked()
{
	int i;
	for(i = 0 ; i < MYTBFMAX ; i++)
	{
		if(job[i] == NULL)
			return i;
	}
	return -1;
}

static void * thr_func(void *p)
{
	int i ;

	while(1)
	{
		pthread_mutex_lock(&mut_job);
		for(i = 0 ; i < MYTBFMAX ; i++)
		{
			if(job[i] != NULL)
			{
				//加两把锁是没有问题的。
				pthread_mutex_lock(&job[i]->mut_token);
				job[i]->token += job[i]->cps;
				if(job[i]->token > job[i]->burst)
					job[i]->token = job[i]->burst;
				pthread_mutex_unlock(&job[i]->mut_token);
			}
		}
		pthread_mutex_unlock(&mut_job);
		sleep(1);
	}
}

static void module_unload(void)
{
	int i;

//不需要加锁的
//	pthread_mutex_lock(&mut_job);
	for(i = 0 ; i < MYTBFMAX ; i++)
	{
		if(job[i] != NULL)
		{
		//	pthread_mutex_destroy(&job[i]->mut_token);
		//	free(job[i]);
			mytbf_destroy(job[i]);
		}
	}
//	pthread_mutex_unlock(&mut_job);

	//先cancel 后join，一定要注意，正在运行的线程是无法直接join的
	pthread_cancel(tid);
	pthread_join(tid,NULL);
}

static void module_load(void)
{
	int err;

	err = pthread_create(&tid,NULL,thr_func,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create %s\n",strerror(err));
		exit(1);
	}
	
	atexit(module_unload);
}


mytbf_t * mytbf_init(int cps,int burst)
{
	int pos;
	struct mytbf_st *me;

	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;

	pthread_mutex_lock(&mut_job);
	pos = get_free_pos_unlocked();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_job);
		free(me);
		return NULL;
	}
	job[pos] = me;
	me->pos = pos;
	pthread_mutex_unlock(&mut_job);

	pthread_mutex_init(&(me->mut_token),NULL);
	me->cps = cps;
	me->token = 0;
	me->burst = burst;

	//此处可能产生竞争
	/*
	pthread_mutex_lock(&mut_job);
	if(inited == 0)
	{
		module_load();
		inited = 1;
	}
	pthread_mutex_unlock(&mut_job);
	*/

	//动态模块的单次初始化
	
	pthread_once(&once_init,module_load);

	return me;
}

static int min_unlocked(int a,int b)
{
	if(a < b)
		return a;
	return b;
}

int mytbf_fetchtoken(mytbf_t *ptr,int size)
{
	if(size < 0)
		return -EINVAL;
	struct mytbf_st *me = ptr;
	int n;

	pthread_mutex_lock(&(me->mut_token));
	while(me->token <= 0)
	{
		pthread_mutex_unlock(&(me->mut_token));
		sched_yield();
		pthread_mutex_lock(&(me->mut_token));
	}
	
	n = min_unlocked(me->token,size);
	me->token -= n;
	pthread_mutex_unlock(&(me->mut_token));

	return n;
}

int mytbf_returntoken(mytbf_t *ptr,int size)
{
	if(size < 0)
		return -EINVAL;
	struct mytbf_st *me  = ptr;

	pthread_mutex_lock(&me->mut_token);
	me->token += size;
	if(me->token > me->burst)
		me->token = me->burst;
	pthread_mutex_unlock(&me->mut_token);
	return size;
}

int mytbf_destroy(mytbf_t *ptr)
{
	struct mytbf_st *me = ptr;
	pthread_mutex_lock(&mut_job);
	job[me->pos] = NULL;
	pthread_mutex_unlock(&mut_job);
	free(ptr);

}


