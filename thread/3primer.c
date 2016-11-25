#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#define			LEFT		30000000
#define			RIGHT		30000200
#define			THRNUM		(RIGHT - LEFT + 1)
/*
 *	注意，会产生竞争的现象
 *
 *
 */
static void *thr_primer(void *p);

int main()
{
	int i,err;
	pthread_t tid[THRNUM];

	for(i = LEFT ; i < RIGHT ; i++)
	{
		err = pthread_create(tid+(i-LEFT),NULL,thr_primer,(void *)i);
		if(err)
		{
			fprintf(stderr,"%s\n",strerror(err));
			exit(1);
		}

	}

	for(i = LEFT ; i < RIGHT ; i++)
	{
		pthread_join(tid[i-LEFT],NULL);
	}

	exit(0);
}


static void *thr_primer(void *p)
{
	int i = (int)p;
	int j,mark;

	mark =1;
	for(j = 2; j < i/2 ; j++)
	{
		if(i % j == 0)
		{
			mark = 0; 
			break;
		}

	}
	if(mark)
		printf("%d is primer\n",i);

	pthread_exit(NULL);	
}



