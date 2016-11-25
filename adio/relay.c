#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//缓冲区大小 
#define			BUFSIZE			10

//打开的两个虚拟终端
#define			TTY1			"/dev/tty11"
#define			TTY2			"/dev/tty12"

enum
{
	//读态
	STATE_R = 1,
	//写态
	STATE_W,
	//异常态
	STATE_Ex,
	//退出态
	STATE_T
};

struct fsm_st
{
	//源文件描述符
	int sfd;
	//目的文件描述符
	int dfd;
	//写到的位置，坚持写够len个字节
	int pos;
	//读取到len个字节
	int len;
	//状态机的状态
	int state;
	//从fd中读取的内容存储到buf缓冲区中
	char buf[BUFSIZE];
	//记录出错信息
	char *errstr;
};

static void fsm_driver(struct fsm_st *fsm)
{
	int ret;//状态机写的结果
	switch(fsm->state)
	{
		case STATE_R:
			fsm->len = read(fsm->sfd,fsm->buf,BUFSIZE);
			if(fsm->len < 0)
			{
				if(errno == EAGAIN) //非阻塞假错
					fsm->state = STATE_R;
				else				//非阻塞真错
				{
					fsm->errstr = "read()";
					fsm->state = STATE_Ex;
				}
			}else if(fsm->len == 0) //读完后进入T态
				fsm->state = STATE_T;
				else				//正确读，进入写态
				{
					fsm->pos = 0;	//把要写的位置置为0  坚持写够len个字节
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
				fsm->pos += ret;
				fsm->len -= ret;
				fsm->state = STATE_R;
			}
			break;
		case STATE_Ex:
			perror(fsm->errstr);
			fsm->state = STATE_T;
			break;
		case STATE_T:
			/*****/
			break;
		default:
			abort();//异常，产生coredump文件
			break;
	}
}

static void relay(int fd1,int fd2)
{
	int fd1_save,fd2_save;
	//fsm12 读左写右
	//fsm21 读右写左
	struct fsm_st fsm12,fsm21;

	//不能期待用于一定是用非阻塞的方式打开文件的
	fd1_save = fcntl(fd1,F_GETFL);
	fcntl(fd1,F_SETFL,fd1_save|O_NONBLOCK);

	fd2_save = fcntl(fd2,F_GETFL);
	fcntl(fd2,F_SETFL,fd2_save|O_NONBLOCK);

	//状态机数据结构初始化
	fsm12.sfd = fd1;
	fsm12.dfd = fd2;
	fsm12.pos = 0;
	fsm12.len = 0;
	//初始状态为读态
	fsm12.state = STATE_R;
	fsm12.errstr = NULL;

	fsm21.sfd = fd2;
	fsm21.dfd = fd1;
	fsm21.pos = 0;
	fsm21.len = 0;
	fsm21.state = STATE_R;
	fsm21.errstr = NULL;

	while(fsm12.state != STATE_T || fsm21.state != STATE_T)
	{
		fsm_driver(&fsm12);
		fsm_driver(&fsm21);
	}

	//记住要恢复文件描述的属性，永远是在写一个模块
	
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


}


