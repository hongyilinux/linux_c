#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

#define		BUFSIZE			1024
#define		TTY1			"/dev/tty11"
#define		TTY2			"/dev/tty12"


enum
{
	STATE_R =1,
	STATE_W,
	STATE_AUTO,
	STATE_Ex,
	STATE_T
};

struct fsm_st
{
	int sfd;
	int dfd;
	char buf[BUFSIZE];
	int len;
	int pos;
	int state;
	char *errstr;
};

static void fsm_driver(struct fsm_st *fsm)
{
	int ret;
	switch(fsm->state)
	{
		case STATE_R:
			fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
			if(fsm->len < 0)
			{
				if(errno == EAGAIN)
					fsm->state = STATE_R;
				else
				{
					fsm->errstr = "read() err";
					fsm->state = STATE_Ex;
				}
			}else if(fsm->len == 0)
				fsm->state = STATE_T;
			else
			{
				fsm->pos = 0;
				fsm->state = STATE_W;
			}
			break;
		case STATE_W:
			ret = write(fsm->dfd,fsm->buf+fsm->pos,fsm->len);
			if(ret < 0)
			{
				if(errno == EAGAIN)
					fsm->state = STATE_W;
				else
				{
					fsm->errstr = "write() err";
					fsm->state = STATE_Ex;
				}
			}else
			{
				fsm->len -= ret;
				fsm->pos += ret;
				if(fsm->len > 0)
					fsm->state = STATE_W;
			}
			break;
		case STATE_Ex:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			break;
		default:
			abort();
			break;
	}

}

static void relay(int fd1,int fd2)
{
	int fd1_save,fd2_save;
	struct fsm_st fsm12,fsm21;
	int epfd;							//初始化epoll_create;
	struct epoll_event ev;				//定义epoll时间结构体	

	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save | O_NONBLOCK);

	fd2_save = fcntl(fd2,F_GETFL);
	fcntl(fd2,F_SETFL,fd2_save | O_NONBLOCK);
	
	fsm12.sfd = fd1;
	fsm12.dfd = fd2;
	fsm12.state = STATE_R;
	fsm12.errstr = NULL;
	fsm12.len = 0;
	fsm12.pos = 0;

	fsm21.sfd = fd2;
	fsm21.dfd = fd1;
	fsm21.state = STATE_R;
	fsm21.errstr = NULL;
	fsm21.len = 0;
	fsm21.pos = 0;
	
	epfd = epoll_create(10); //10是一个随意给定值，没有要求，系统用于hash
	if(epfd < 0)
	{
		perror("epoll_create()");
		exit(1);
	}
	
	//在while循环外add，避免每次监视add，监视后只需要根据相应结果MOD即可
	//理解union的含义

	ev.events = 0;			//位图初始化为0
	ev.data.fd = fd1;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd1,&ev);

	ev.events = 0;
	ev.data.fd = fd2;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd2,&ev);

	while(fsm12.state != STATE_T && fsm21.state != STATE_T)
	{
		//布置监视现场
		ev.events = 0;
		ev.data.fd = fd1;
		if(fsm12.state == STATE_R)	//一可读
			ev.events |= EPOLLIN;
		if(fsm21.state == STATE_W)  //二可写
			ev.events |= EPOLLOUT;
		
		//调整。MOD,提高程序运行效率
		epoll_ctl(epfd,EPOLL_CTL_MOD,fd1,&ev);
		
		ev.events = 0;
		ev.data.fd = fd2;
		if(fsm21.state == STATE_R) 		//二可读
			ev.events |= EPOLLIN;
		if(fsm12.state == STATE_W)		//一可写
			ev.events |= EPOLLOUT;
		epoll_ctl(epfd,EPOLL_CTL_MOD,fd2,&ev);

		//监视，epoll_wait函数,ev数据结构中，监视事件和结果事件分开，故可以while小圈循环
		//最后一个参数-1表明死等，阻塞方式
		if(fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
		{
			while(epoll_wait(epfd,&ev,1,-1) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("epoll_wait()");
				exit(1);
			}
		}

		//根据监视结果推动状态机
		if(ev.data.fd == fd1 && ev.events & EPOLLIN \
			|| ev.data.fd == fd2 && ev.events & EPOLLOUT \
			|| fsm12.state > STATE_AUTO)
			fsm_driver(&fsm12);
		if(ev.data.fd == fd2 && ev.events & EPOLLIN \
			|| ev.data.fd == fd1 && ev.events &EPOLLOUT \
			|| fsm21.state > STATE_AUTO)
			fsm_driver(&fsm21);
	}

	close(epfd);
	fcntl(fd1,F_SETFL,fd1_save);
	fcntl(fd2,F_SETFL,fd2_save);
}

int main()
{
	int fd1,fd2;
	fd1 = open(TTY1,O_RDWR);
	if(fd1 < 0)
	{
		perror("open()");
		exit(1);
	}
	write(fd1,"TTY1\n",5);

	fd2 = open(TTY2,O_RDWR);
	if(fd2 < 0)
	{
		perror("open()");
		exit(1);
	}
	write(fd2,"TTY2\n",5);

	relay(fd1,fd2);

	close(fd1);
	close(fd2);
	exit(0);
}




