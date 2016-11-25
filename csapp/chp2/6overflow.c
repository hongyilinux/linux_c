#include <stdio.h>
#include <stdlib.h>
/*
 * 	float arr[]={};如果是空数组会出错
 * 	原因：
 * 		usigned length，0-1 会产生一个巨大的整数
 * 	办法：
 * 		1、i < length 而不要写成 i <= length-1
 *		2、usigned length 定义成 int length
 *
 *
 */

float sum_elements(float a[] , unsigned length)
{
	int i;

	float result = 0;

	for(i = 0 ; i <= length -1 ; i++)
		result += a[i];
	
	return result;
}


int main()
{
	float arr[]={};

	printf("%f\n",sum_elements(arr,sizeof(arr)/sizeof(arr[0])));

	exit(0);
}


