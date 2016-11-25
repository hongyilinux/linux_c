#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mypipe.h"

struct mypipe_st
{
	int header;
	int tail;
	char buf[BUFSIZE];
	//管道长度
	int pipesize;
	// 管道的读者数量,管道必须凑齐读写双方，方才有效
	int count_rd;
	 //管道的写着数量
	int count_wr;
	//管道有可能被多个进程或线程使用，需要定义互斥变量
	pthread_mutex_t mut;
	pthread_cond_t cond;
};

mypipe_t * mypipe_init()
{
	struct mypipe_st *me;
	me = malloc(sizeof(*me));
	if(me == NULL)
		return NULL;
	
	me->header = 0;
	me->tail = 0;
	me->pipesize = 0;
	me->count_rd = 0;
	me->count_wr = 0;
	pthread_mutex_init(&me->mut,NULL);
	pthread_cond_init(&me->cond,NULL);

	return me;
}

int mypipe_register(mypipe_t *ptr,int op)
{
	struct mypipe_st *me = ptr;
	//传入的参数非法
	if(op & PIPE_RD || op & PIPE_WR)
		return -1;
	
	pthread_mutex_lock(&me->mut);
	
	if(op & PIPE_RD)
		me->count_rd++;
	if(op & PIPE_WR)
		me->count_wr++;
	
	pthread_cond_broadcast(&me->cond);
	while(me->count_rd <= 0 || me->count_wr <= 0)
		pthread_cond_wait(&me->cond,&me->mut);

	pthread_mutex_unlock(&me->mut);

	return 0;

}

int mypipe_unregister(mypipe_t *ptr,int op)
{
	struct mypipe_st *me = ptr;
	if(op & PIPE_RD || op & PIPE_WR)
		return -1;
	
	pthread_mutex_lock(&me->mut);

	if(op & PIPE_RD)
		me->count_rd--;
	if(op & PIPE_WR)
		me->count_wr--;
	
	pthread_cond_broadcast(&me->cond);
	pthread_mutex_unlock(&me->mut);
	return 0;
}

static int min_unlock(int a,int b)
{
	if(a < b)
		return a;
	return b;
}

static int  read_by_byte_unlocked(struct mypipe_st *me,char  *buf)
{
	if(me->pipesize <= 0)
		return -1;

	pthread_mutex_lock(&me->mut);
	
	*buf = me->buf[me->header];
	me->header = next(me->header);
	me->pipesize--;
	pthread_mutex_unlock(&me->mut);

	return 0;
}

//仿照read系统调用写
int mypipe_read(mypipe_t *ptr,void *buf,const int count)
{
	int i;
	struct mypipe_st *me = ptr;
	
	pthread_mutex_lock(&me->mut);

	//如果管道没有内容，且管道中的写者数量大于0，需要等待
	while(me->pipesize <= 0 && me->count_wr > 0)
		pthread_cond_wait(&me->cond,&me->mut);
	
	//管道中没有内容，且没有写着，那么管道读者退出
	if(me->pipesize <= 0 && me->count_wr <= 0)
	{
		pthread_mutex_unlock(&me->mut);
		return 0;
	}


	for(i = 0 ; i < count ; i++)
	{
		if(read_by_byte_unlocked(me,buf+i) != 0)
			break;//跳出for循环
	}

	pthread_mutex_unlock(&me->mut);

	return i;
}

int mypipe_write(mypipe_t *ptr,void *buf,const int count)
{

}

int mypipe_destroy(mypipe_t *ptr)
{
	struct mypipe_st *me = ptr;
	pthread_mutex_destroy(&me->mut);
	pthread_cond_destroy(&me->cond);
	free(ptr);
	return 0;
}

