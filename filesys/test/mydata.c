#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

/*
 *功能：求出100天以后，是几月几号
*	测试命令行传参。getopt(3)   getopt_long(3)
 *	命令行的参数：
 *  -y	:	year
 *	-m	:	monh
 *	-d	:	day
 *	-H	:	hour
 *	-M	:	minute
 *	-S	:	second
 *	思考如何支持长格式传参
 *
 */

#define	TIMESTRSIZE		1024
#define FMTSTRSIZE		1024
int main(int argc,char **argv)
{
	FILE *fp = stdout;
	time_t stamp;
	struct tm  *tm;
	int c;
	stamp = time(NULL);
	char timestr[TIMESTRSIZE];
	char fmtstr[FMTSTRSIZE];
	fmtstr[0] = '\0';
	tm = localtime(&stamp);

	while(1)
	{
		//如果："-HMSymd"可以接受非选项传参
		//如果："H:MSy:md" 可以接受选项参数
		//如果: "HMSymd"，只能接受选项
		c = getopt(argc,argv,"-H:MSy:md");			//命令行分析加冒号用上了全局变量
		if(c < 0)
			{
				break;
			}
		switch(c)
		{
			//如何实现带参数的选项
			case 1:
				fp = fopen(argv[optind-1],"w");
				if(fp == NULL)
				{
					perror("fopen()");
					fp = stdout ;   //如果失败，写入终端
				}
				break;
			case 'H':
				if(strcmp(optarg,"12") == 0)
				{
					strncat(fmtstr,":%I(%P)",FMTSTRSIZE);
				}else if(strcmp(optarg,"24") == 0)
				{
					strncat(fmtstr,"%H",FMTSTRSIZE);				
				} else
				{
					fprintf(stderr,"arg is wrong\n");
				}
				break;
			case 'M':
				strncat(fmtstr,":%M",FMTSTRSIZE);			
				break;
			case 'S':
				strncat(fmtstr,":%S",FMTSTRSIZE);
				break;
			case 'y':
				if(strcmp(optarg,"2") == 0)
				{
					strncat(fmtstr,"%y",FMTSTRSIZE);
				}else if(strcmp(optarg,"4") == 0)
				{					
					strncat(fmtstr,"%Y",FMTSTRSIZE);
				}else
				{
					fprintf(stderr,"arg is wrong\n");
				}
				break;
			case 'm':
				strncat(fmtstr,"-%m",FMTSTRSIZE);
				break;
			case 'd':
				strncat(fmtstr,"-%d",FMTSTRSIZE);
				break;
			default:
				break;
		}	
	}

	strncat(fmtstr,"\n",FMTSTRSIZE);
	strftime(timestr,TIMESTRSIZE,fmtstr,tm);	
	
	fputs(timestr,fp);	
	
	
	

	exit(0);
}










