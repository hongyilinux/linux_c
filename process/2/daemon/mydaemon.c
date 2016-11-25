#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
/*	注意守护进程的特点
 *	注意主日志文件的位置 /var/log/messages
 *	日志文件的配置文件：
 *	/etc/sysconfig/rsyslog
 *
 */

#define 	FNAME 	"/tmp/out"

static int daemonize(void)
{
	pid_t pid;
	int fd;
	pid = fork();
	if(pid < 0)
	{
		return -1;		//main函数中写了日志了
	}
	if(pid > 0)
		exit(0);
	fd = open("/dev/null",O_RDWR);
	if(fd < 0)
	{
		return -1;
	}
	dup2(fd,0);
	dup2(fd,1);
	dup2(fd,2);
	if(fd > 2)
		close(fd);
	setsid();		//创建守护进程
	chdir("/");		//防止守护进程踩着某设备
	umask(0);		//可有可无

	return 0;
}

int main()
{
	int ret;
	pid_t pid;
	openlog("mydaemon",LOG_PID,LOG_DAEMON);
	int i;
	ret = daemonize();		//实现守护进程
	if(ret != 0)
	{
		syslog(LOG_ERR,"daemonize failed!");
		exit(1);
	}else
	{
		syslog(LOG_INFO,"daemonize() successded");			//不需要\n
	}	
	FILE *fp;

	fp = fopen(FNAME,"w");
	
	if(fp == NULL)
	{
		syslog(LOG_ERR,"fopen:%s",strerror(errno));
		exit(1);
	}
	syslog(LOG_INFO,"%s was opened .",FNAME);
	for(i = 0 ; ; i++)
	{
		fprintf(fp, "%d\n",i);
		syslog(LOG_DEBUG,"%i is printed",i);
		fflush(NULL);
		sleep(1);	
	}
	
	fclose(fp);		
	closelog();
	exit(0);
}





