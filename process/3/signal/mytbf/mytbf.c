#include <stdio.h>
#include <stdlib.h>
#include "mytbf.h"

struct mytbf_st
{
	int cps;
	int burst;
	int token;
	int pos;
};

//定义全局变量数组

static struct mytbf_st * job[MTBF_MAX];

static int get_free_pos(void)
{
	int i;
	for(i = 0; i < MYTBF_MAX ; i++)
	{
		if(job[i] == NULL)
			return i;
	}
	return -1;   //表示没有空位
}

mytbf_t *mytbf_init(int cps,int burst)
{
	struct mytbf_st *me;

	int pos ;

	pos = get_free_position();
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

int mytbf_fetchtoken(mytbf_t *ptr ,int size)
{
	if(size <=0)
	{

	}
}

int mytbf_returntoken(mytbf_t *,int)
{

}

int mytbf_destroy(mytbf_t *ptr)
{
	struct mytbf_st *me = ptr;

	job[me->pos] = NULL;

	free(ptr);

	return 0;
}


