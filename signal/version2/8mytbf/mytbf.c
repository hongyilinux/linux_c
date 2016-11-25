#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "mytbf.h"

typedef	void(*sighandler_t)(int);

struct mytbf
{
	int cps;
	int burst;
	int token;
	int pos;
};

static struct mytbf * job[MYTBF_MAX];
static int inited  = 0;
static sighandler_t alrm_handler_save;

static int get_free_position()
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

	for(i = 0 ; i < MYTBF_MAX ; i++)
	{
		if(job[i] != NULL)
		{
			job[i]->token += job[i]->cps;
			if(job[i]->token >= job[i]->burst)
				job[i]->token = job[i]->burst;
		}
	}
}

static void module_unload(void)
{
	int i;
	alarm(0);
	signal(SIGALRM,alrm_handler_save);
	for(i = 0 ; i < MYTBF_MAX ; i++)
	{
		free(job[i]);
	}
}

static void module_load(void)
{
	alrm_handler_save = signal(SIGALRM,alrm_handler);
	alarm(1);
	atexit(module_unload);
}

mytbf_t *  mytbf_init(int cps,int burst)
{
	struct mytbf *me;
	int pos;
	
	if(inited == 0)
	{
		module_load();
		inited = 1;
	}


	pos = get_free_position();
	if(pos == -1)
		return NULL;
	me = malloc(sizeof(*me));
	if(me == NULL)
		return me;
	
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
	struct mytbf *me = ptr;
	int ret_token_size;

	if(size < 0)
		return -EINVAL;
	while(me->token <= 0)
		pause();
	
	ret_token_size = min(me->token ,size);

	me->token -= ret_token_size;

	return ret_token_size;
}

int mytbf_returntoken(mytbf_t *ptr,int size)
{
	struct mytbf *me = ptr;

	if(size < 0)
		return -EINVAL;
	
	me->token += size;

	if(me->token > me->burst)
		me->token = me->burst;
	return size;
}

int mytbf_destroy(mytbf_t *ptr)
{
	struct mytbf *me = ptr;

	job[me->pos] = NULL;

	free(ptr);
	return 0;
}

