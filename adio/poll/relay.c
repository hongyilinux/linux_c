#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

#define			TTY1		"/dev/tty11"
#define			TTY2		"/dev/tty12"
#define			BUFSIZE		1024

enum
{
	STATE_R = 1,
	STATE_W,
	STATE_AUTO,
	STATE_Ex,
	STATE_T
};

struct fsm_st
{
	int sfd;
	int dfd;
	int state;
	int pos;
	int len;
	char buf[BUFSIZE];
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
					fsm->errstr = "read";
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
					fsm->errstr = "write";
					fsm->state = STATE_Ex;
				}
			}else
			{
				fsm->len -= ret;
				fsm->pos += ret;
				if(fsm->len > 0)
					fsm->state = STATE_W;
				else
					fsm->state = STATE_R;
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
	int fd1_save;
	int fd2_save;
	struct fsm_st fsm12,fsm21;
	// 定义文件描述符数组
	struct pollfd pfd[2];

	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save | O_NONBLOCK);

	fd2_save = fcntl(fd2,F_GETFL);
	fcntl(fd2,F_SETFL,fd2_save | O_NONBLOCK);

	fsm12.sfd = fd1;
	fsm12.dfd = fd2;
	fsm12.len = 0; 
	fsm12.pos = 0;
	fsm12.state = STATE_R;
	fsm12.errstr = NULL;

	fsm21.sfd = fd2;
	fsm21.dfd = fd1;
	fsm21.len = 0; 
	fsm21.pos = 0;
	fsm21.state = STATE_R;
	fsm21.errstr = NULL;

	//文件描述符装载，写在循坏外面，有助于提高程序效率	
	pfd[0].fd = fd1;
	pfd[1].fd = fd2;
	while(fsm12.state != STATE_T || fsm21.state != STATE_T)
	{
		//布置监视现场
		//auto变量，需要清零
		pfd[0].events = 0;
		pfd[0].revents = 0;
		pfd[1].events = 0;
		pfd[1].revents = 0; 
		if(fsm12.state == STATE_R)
			pfd[0].events |= POLLIN;
		if(fsm12.state == STATE_W)
			pfd[1].events |= POLLOUT;
		if(fsm21.state == STATE_R)
			pfd[1].events |= POLLIN;
		if(fsm21.state == STATE_W)
			pfd[0].events |= POLLOUT;

		//监视
		if(fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
		{
			//最后一个参数为-1表明是阻塞模式，可能被信号打断，需要判断EINTR
			//监视事件events和返回时间revents是分开存放的，故需要转小圈，提高程序效率
			while(poll(pfd,2,-1) < 0)
			{
				if(errno == EINTR)
					continue;
				perror("poll");
				exit(1);
			}
		}

		//根据监视结果推动状态机
		if((pfd[0].revents & POLLIN) || (pfd[1].revents & POLLOUT) || fsm12.state > STATE_AUTO)
			fsm_driver(&fsm12);

		if((pfd[1].revents & POLLIN) || (pfd[0].revents & POLLOUT) || fsm21.state > STATE_AUTO)
			fsm_driver(&fsm21);
	}
	
	fcntl(fd1,F_SETFL,fd1_save);
	fcntl(fd2,F_SETFL,fd2_save);

}

int main()
{
	int fd1,fd2;

	fd1 = open(TTY1,O_RDWR);
	if(fd1 < 0)
	{
		perror("open");
		exit(1);
	}
	write(fd1,"TTY1\n",5);

	fd2 = open(TTY2,O_RDWR);
	if(fd2 < 0)
	{
		perror("open");
		exit(1);
	}
	write(fd2,"TTY2\n",5);

	relay(fd1,fd2);

	close(fd1);
	close(fd2);
	exit(0);
}

