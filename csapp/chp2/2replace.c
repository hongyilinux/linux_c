#include <stdio.h>
#include <stdlib.h>

void inplace_swap(int *x,int *y)
{
	*y = *x ^ *y;
	*x = *x ^ *y;
	*y = *x ^ *y;
}

void reverse_array(int a[],int cnt)
{
	int first,last;

	for(first = 0, last = cnt -1;first < last ; first++,last-- )
	{
		inplace_swap(&a[first],&a[last]);
	}
}

void print_array(int a[],int cnt)
{
	int i;
	for(i = 0 ; i < cnt ; i++)
	{
		printf("%d ",a[i]);
	}
	printf("\n");
}

int main()
{
	int x = 5;

	int y = 6;
	
	printf("x = %d ; y = %d\n",x,y);
	inplace_swap(&x,&y);
	printf("x = %d ; y = %d\n",x,y);

	int intarr[]= {1,3,5,7,9};

	print_array(intarr,sizeof(intarr)/sizeof(intarr[0]));

	reverse_array(intarr,sizeof(intarr)/sizeof(intarr[0]));

	print_array(intarr,sizeof(intarr)/sizeof(intarr[0]));

	exit(0);
}


