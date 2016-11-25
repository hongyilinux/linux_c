#ifndef		MYPIPE_H__
#define		MYPIPE_H__

#define		BUFSIZE				1024

//位图
#define		PIPE_RD				0X00000001UL
#define		PIPE_WR				0X00000002UL

typedef		void 		mypipe_t;

mypipe_t *  mypipe_init();

int mypipe_register(mypipe_t *ptr,int op);

int mypipe_unregister(mypipe_t *ptr,int op);

//仿照read系统调用写
int mypipe_read(mypipe_t *ptr,void *buf,const int count);

int mypipe_write(mypipe_t *ptr,void *buf,const int count);

int mypipe_destroy(mypipe_t *ptr);

#endif
