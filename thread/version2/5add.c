#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define		THRNUM		20
#define		FNAME		"/tmp/out"
#define		BUFSIZE		1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void * thr_func(void *p)
{
	FILE *fp;
	char buf[BUFSIZE];

	fp = fopen(FNAME,"r+");
	if(fp == NULL)
	{
		perror("fopen");
		exit(1);
	}

	pthread_mutex_lock(&mut);
	fgets(buf,BUFSIZE,fp);

	//注意，一定要fseek，否则文件位置指针的位置还是在文件尾部
	fseek(fp,0,SEEK_SET);

	//printf("%d\n",atoi("1\n"));
	//打印结果是1
	//printf("%d\n",atoi("\n"));
	//打印结果是0
	//总结说来就是aoti字符串的时候，结尾的\n是不输出的
	fprintf(fp,"%d\n",atoi(buf)+1);

	fclose(fp);
	pthread_mutex_unlock(&mut);
	pthread_exit(NULL);

}

int main()
{
	int i,err;
	pthread_t tid[THRNUM];

	for(i = 0 ; i < THRNUM ; i++)
	{
		err = pthread_create(tid+i,NULL,thr_func,NULL);
		if(err)
		{
			fprintf(stderr,"pthread_create()%s\n",strerror(err));
			exit(1);
		}
	}

	for(i = 0 ; i < THRNUM ; i++)
	{
		pthread_join(tid[i],NULL);
	}

	
	pthread_mutex_destroy(&mut);
	exit(0);
}



