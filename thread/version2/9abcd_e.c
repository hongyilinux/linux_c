#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define		THRNUM		4

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

static int next(int i)
{
	if(i+1 == THRNUM)
		return 0;
	return i+1;
}

static void * thr_func(void *p)
{
	int n = (int)p;
	int ch = 'a' + n;

	while(1)
	{
		pthread_mutex_lock(&mut);
		while(n != num)
			pthread_cond_wait(&cond,&mut);
		write(1,&ch,1);
		num = next(num);
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mut);
	}

	pthread_exit(NULL);
}

int main()
{
	int i, err;
	pthread_t tid[THRNUM];
	
	alarm(3);

	for(i = 0 ; i < THRNUM ; i++)
	{
		err = pthread_create(tid+i,NULL,thr_func,(void *)i);
		if(err)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	for(i = 0 ; i < THRNUM ; i++)
		pthread_join(tid[i],NULL);

	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);
	exit(0);

}


