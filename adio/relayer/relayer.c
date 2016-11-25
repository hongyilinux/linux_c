#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "relayer.h"

//定义缓冲区长度
#define		BUFSIZE			1024

enum
{
	STATE_R = 1,
	STATE_W,
	STATE_Ex,
	STATE_T
};

//定义状态机数据结构
struct rel_fsm_st
{
	//状态机状态
	int state;
	//源文件描述符
	int sfd;
	//目的文件描述符
	int dfd;
	//写文件描述符的位置信息 ，读写都需要
	int pos;
	//读取信息长度
	int len;
	//读取信息的存储缓存空间
	char buf[BUFSIZE];
	//出错信息记录
	char *errstr;
};

//定义job任务数组的数据结构
struct rel_job_st
{
	//任务位置自述
	int pos;
	//文件描述符1
	int fd1;
	//文件描述符2
	int fd2;
	//文件描述符1初始状态
	int fd1_save;
	//文件描述符2初始状态
	int fd2_save;
	//状态机1，读左写右
	struct rel_fsm_st fsm12;
	//状态机2，读右写左
	struct rel_fsm_st fsm21;
	//任务状态
	int state;
	//任务传输字节数
	int64_t count;
};

//任务数组，每一个位置初始位空
static struct rel_job_st * job[REL_JOB_MAX];

//线程id,构造一个线程，专门用于推动状态机
static pthread_t tid_relayer;

//单次初始化变量
static pthread_once_t once_inited = PTHREAD_ONCE_INIT;

//job因公用，会产生竞争，需要加锁 
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;

static int get_free_pos_unlocked()
{
	int i;
	for(i = 0; i < REL_JOB_MAX ; i++)
	{
		if(job[i] == NULL)
			return i;
	}
	return -1;
}

static void rel_fsm_driver(struct rel_fsm_st *fsm)
{
	int ret;

	switch(fsm->state)
	{
		case STATE_R:
			fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
			if(fsm->len < 0)
			{
				if(errno == EAGAIN)			//假错
					fsm->state = STATE_R;
				else						//真错
				{
					fsm->errstr = "read()";
					fsm->state = STATE_Ex;
				}
			}else if(fsm->len == 0)			//读取结束
			{
				fsm->state = STATE_T;
			}else							//正常读取
			{
				//设置读取位置,并将状态机状态设置为写态
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
			break;

		case STATE_W:
			ret = write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
			if(ret < 0)
			{
				if(errno == EAGAIN)			//假错
					fsm->state = STATE_W;
				else
				{
					fsm->errstr = "write()";
					fsm->state = STATE_Ex;
				}
			}else
			{
				fsm->len -= ret;
				fsm->pos += ret;
				if(fsm->len > 0)
					fsm->state = STATE_W;  // 坚持写够len个字节
				else
					fsm->state = STATE_R;	//完成坚持写够len个字节，转入读态
			}
			break;

		case STATE_Ex:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;

		case STATE_T:
			/****/
			break;

		default:
			abort();
			break;
	}
}


// 线程函数，始终运行，推动状态机
static void * thr_func(void *p)
{
	int i;
	pthread_mutex_lock(&mut_job);
	while(1)
	{
		for(i = 0 ; i < REL_JOB_MAX ; i++)
		{
			if(job[i] != NULL)
			{
				if(job[i]->state == STATE_RUNING)
				{
					rel_fsm_driver(&job[i]->fsm12);
					rel_fsm_driver(&job[i]->fsm21);
					//如果job中的两个状态机都是T态，那么job运行结束，修改job的状态为STATE_OVERED
					if(job[i]->fsm12.state == STATE_T && job[i]->fsm21.state == STATE_T)
						job[i]->state = STATE_OVERED;
				}
			}
		}

		pthread_mutex_unlock(&mut_job);
	}
}

static void module_load(void)
{
	int err;

	err = pthread_create(&tid_relayer,NULL,thr_func,NULL);
	if(err)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(err));
		exit(1);
	}

	//挂载钩子函数
//	atexit(module_unload);

}

rel_addjob(int fd1,int fd2)
{
	struct rel_job_st *me;
	int pos;
	
	//单次加载模块
	pthread_once(&once_inited,module_load);
	
	me = malloc(sizeof(*me));
	if(me == NULL)
		return -ENOMEM;
	
	pthread_mutex_lock(&mut_job);
	
	pos = get_free_pos_unlocked();
	if(pos < 0)
	{
		pthread_mutex_unlock(&mut_job);
		free(me);
		return -ENOSPC;
	}
	me->pos = pos;
	job[pos] = me;	
	pthread_mutex_unlock(&mut_job);
	
	//确保非阻塞方式
	me->fd1 = fd1;
	me->fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,me->fd1_save | O_NONBLOCK);

	me->fd2 = fd2;
	me->fd2_save = fcntl(fd2,F_GETFL);
	fcntl(fd2,F_SETFL,me->fd2_save | O_NONBLOCK);

	me->state = STATE_RUNING;
	me->count = 0;

	me->fsm12.sfd = fd1;
	me->fsm12.dfd = fd2;
	me->fsm12.pos = 0;
	me->fsm12.len = 0;
	me->fsm12.state = STATE_R;
	me->fsm12.errstr = NULL;

	me->fsm21.sfd = fd2;
	me->fsm21.dfd = fd1;
	me->fsm21.pos = 0;
	me->fsm21.len = 0;
	me->fsm21.state = STATE_R;
	me->fsm21.errstr = NULL;

	return me->pos;
}
