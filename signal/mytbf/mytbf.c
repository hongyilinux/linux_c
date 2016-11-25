#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "mytbf.h"


typedef void (*sighandler_t)(int);
static struct mytbf_st* job[MYTBF_MAX];
static int inited = 0;
static sighandler_t alrm_handler_save;

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;    //结构体，在数组中的下标
};

static int get_free_pos(void)
{
	int i;
	for(i = 0 ; i < MYTBF_MAX ; i++)
	{
		if(job[i] == NULL)
			return i;
	}
	return -1;	
}

static void alrm_handler(int s)
{
	int i;
	alarm(1);
	for(i = 0; i < MYTBF_MAX; i++)
	{
		if(job[i] != NULL)
		{
			job[i]->token += job[i]->cps;
			if(job[i]->token > job[i]->burst)
				job[i]->token = job[i]->burst;
		}

	}
}

static void module_unload(void)
{
	//把信号关闭，如果进程异常结束的话，你写的只是一个模块
	
	signal(SIGALRM,alrm_handler_save);
	alarm(0);
	int i;
	for(i = 0 ; i < MYTBF_MAX ; i++)
	{
		if(job[i] != NULL)
			free(job[i]);
		job[i] = NULL;
	}
	
}


static void  module_load(void)
{
	alrm_handler_save = signal(SIGALRM,alrm_handler);
	alarm(1);

	atexit(module_unload);   //使用钩子函数
}

mytbf_t *mytbf_init(int cps,int burst)
{
	struct mytbf_st *me;
	if(!inited)
	{	
		module_load();
		inited = 1;
	}
	
	int pos;
	pos = get_free_pos();
	if(pos < 0)
		return NULL;
	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;
	me->token = 0;
	me->cps = cps;
	me->burst = burst;
	me->pos = pos;
	job[pos] = me;
	return me;
}

static int min(int a,int b)
{
	return a<b?a:b;
}

int mytbf_fetchtoken(mytbf_t *ptr,int size) //取令牌
{
	int n;
	if(size <= 0)
		return -EINVAL;
	struct mytbf_st *me = ptr;
	while(me->token <= 0)
		pause();
	n = min(me->token,size);
	me->token -=n;
	
	return n;	

}

int mytbf_returntoken(mytbf_t *ptr,int size) //还令牌
{
	if(size <= 0)
		return -EINVAL;
	struct mytbf_st *me = ptr;
	me->token += size;
	if(me->token > me->burst)
		me->token = me->burst;
	return size;	
}

int mytbf_destroy(mytbf_t *ptr)
{
	//把job数组的某个空间设置为空
	struct mytbf_st *me = ptr;
	job[me->pos] = NULL;
	free(ptr);	
	return 0;
}





