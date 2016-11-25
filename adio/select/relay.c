#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

//设置缓冲区大小
#define		BUFSIZE			1024

#define		TTY1			"/dev/tty11"
#define		TTY2			"/dev/tty12"

enum
{
	STATE_R = 1,
	STATE_W,
	//STATE_AUTO是一个占位符，超过STATE_AUTO的状态，是需要无条件推动该状态机
	STATE_AUTO,
	STATE_Ex,
	STATE_T
};

struct fsm_st
{
	int sfd;
	int dfd;
	int pos;
	int len;
	int state;
	char buf[BUFSIZE];
	char *errstr;
};

//推状态机函数
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
					fsm->errstr = "read()";
					fsm->state = STATE_Ex;
				}
			}else if(fsm->len == 0)
			{
				fsm->state = STATE_T;
			}else
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
					fsm->errstr = "write()";
					fsm->state = STATE_Ex;
				}
			}else
			{
				fsm->len -= ret;
				fsm->pos += ret;
				if(fsm->len > 0)
				{
					fsm->state = STATE_W;
				}else
				{
					fsm->state = STATE_R;
				}
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


static int max(int a,int b)
{
	if(a > b)
		return a;
	else
		return b;
}

static void relay(int fd1,int fd2)
{
	int fd1_save;
	int fd2_save;
	struct fsm_st fsm12,fsm21;
	//定义读集，写集，注意rset和wset是auto类型变量
	fd_set rset,wset;

	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save | O_NONBLOCK);

	fd2_save = fcntl(fd2,F_GETFL);
	fcntl(fd2,F_SETFL,fd2_save | O_NONBLOCK);
	
	fsm12.sfd = fd1;
	fsm12.dfd = fd2;
	fsm12.state = STATE_R;
	fsm12.len = 0; 
	fsm12.pos = 0;
	fsm12.errstr = NULL;

	fsm21.sfd = fd2;
	fsm21.dfd = fd1;
	fsm21.state = STATE_R;
	fsm21.len = 0; 
	fsm21.pos = 0;
	fsm21.errstr = NULL;

	while(fsm12.state != STATE_T || fsm21.state != STATE_T)
	{
		//布置监视现场
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(fsm12.state == STATE_R)
			FD_SET(fd1,&rset);
		if(fsm12.state == STATE_W)
			FD_SET(fd2,&wset);
		if(fsm21.state == STATE_R)
			FD_SET(fd2,&rset);
		if(fsm21.state == STATE_W)
			FD_SET(fd1,&wset);

		//设置监视
		if(fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO)
		{
			//注意此处用if而不用while，循环需要绕大圈，
			//因为rset，wset即是监视描述符集，也是结果保留的集合，没有cost修饰
			if(select(max(fd1,fd2)+1,&rset,&wset,NULL,NULL) < 0)
			{
				//select最后一个参数设置为NULL表明是阻塞等，就是一直等到结果才返回，有可能被信号打断
				//故需要用EINTR判断
				if(errno == EINTR)
					continue;
				perror("select()");
				exit(1);
			}

		}
		//根据监视结果推动状态机 
		if(FD_ISSET(fd1,&rset) || FD_ISSET(fd2,&wset) || fsm12.state > STATE_AUTO)
			fsm_driver(&fsm12);
		if(FD_ISSET(fd2,&rset) || FD_ISSET(fd1,&wset) || fsm21.state > STATE_AUTO)
			fsm_driver(&fsm21);
	}

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

