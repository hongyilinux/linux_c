#ifndef		RELAYER_H__
#define		RELAYER_H__

//定义任务总量为一万
#define		REL_JOB_MAX			10000		

enum
{
	STATE_RUNING = 1,
	STATE_CANCELED,
	STATE_OVERED
};

//定义任务状态数据结构
struct rel_job_stat_st
{
	int state;
	int sfd;
	int dfd;
	//甚至可以包括任务执行时间 
};

/*	
 *	return >= 0			任务添加成功，返回值为job数组下标
 *		   == -ENOSPC	失败，job任务数组满，
 *		   == -EINVAL	失败，参数非法
 *		   == -ENOMEM	失败，申请内存空间失败
 *
 * */
int rel_addjob(int fd1,int fd2);


/*
 *	return == 0			成功，指定任务被成功取消
 *		   == -EINVAL	失败，参数非法
 *		   == —EBUSY	失败，指定任务重复取消 
 * */

int rel_canljob(int jobid);

/*
 *	return == 0			成功，指定任务资源成功回收
 *		   == -EINVAL	失败，参数非法
 *		   == -EBUSY	失败，任务运行中，先取消后回收 
 *
 */

int rel_waitjob(int jobid,struct rel_job_stat_st *ptr);


/*
 * 	return  == 0		成功，成功读取任务状态，并回填到结构体数组中
 *			== -EINVAL	失败，参数非法
 *
 */

int rel_statjob(int jobid,struct rel_job_stat_st *ptr);
#endif
