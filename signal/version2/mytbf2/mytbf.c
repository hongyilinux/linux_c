#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "mytbf.h"

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
};

//sighandler_t 属于未定义类型
typedef	void (*sighandler_t)(int);

static sighandler_t alarm_handler_save;

static struct mytbf_st * job[MYTBFMAX];

// 一次调用开关
static int inited = 0;

static int get_free_pos()
{
	int i;
	for(i = 0 ; i < MYTBFMAX ; i++)
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
	for(i = 0 ; i < MYTBFMAX ; i++)
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
	int i;
	alarm(0);
	signal(SIGALRM,alarm_handler_save);
	for(i = 0 ; i < MYTBFMAX ; i++)
	{
		free(job[i]);
	}
}

static void module_load(void)
{
	alarm_handler_save = signal(SIGALRM,alrm_handler);
	alarm(1);
	atexit(module_unload);
}

mytbf_t * mytbf_init(int cps,int burst)
{
	struct mytbf_st *me;
	int pos;
	
	pos = get_free_pos();
	if(pos < 0)
		return NULL;

	if(inited == 0)
	{
		module_load();
		inited = 1;
	}
	me = malloc(sizeof(*me));

	if(me == NULL)
		return NULL;
	
	me->cps = cps;
	me->burst = burst;
	me->token = 0;
	me->pos = pos;
	
	job[pos] = me;
	return me;
}

static int min(int a,int b)
{
	if(a < b)
		return a;
	return b;
}

int mytbf_fetchtoken(mytbf_t *ptr,int size)
{
	int n;
	if(size < 0)
		return -EINVAL;

	struct mytbf_st *me = ptr;

	while(me->token <= 0)
		pause();
	
	n = min(me->token,size);

	me->token -= n;

	return n;
}

int mytbf_returntoken(mytbf_t *ptr,int size)
{
	if(size < 0)
		return -EINVAL;
	struct mytbf_st *me = ptr;

	me->token += size;

	if(me->token > me->burst)
		me->token = me->burst;
	return size;

}

int mytbf_destroy(mytbf_t *ptr)
{
	struct mytbf_st *me = ptr;

	job[me->pos] = NULL;
	free(ptr);
}


