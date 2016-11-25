#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE		1024

//获取指定行的定位指针位置
int  lseek_line_n(FILE *fp,int n)
{
	if( n < 0)
	{
		return -1;
	}

	int ch;
	int c_count = 0;
	int l_count = 0 ;
	
	while ((ch = fgetc(fp)) != EOF)
	{
		c_count++;
		if(ch == '\n')
			++l_count;
		if(l_count == n)
			return c_count;
	}
}


int main(int argc ,char ** argv)
{

	if(argc < 2)
	{
		fprintf(stderr,"Usage..\n");
		exit(1);
	}
	int size =0;
	FILE *fp1,*fp2;
	int ret;
	int count1,count2;
	char buf[BUFSIZE];
	fp1 = fopen(argv[1], "r");
	if(fp1 == NULL)
	{
		perror("fopen");
		exit(1);
	}

	fseek(fp1,0,SEEK_END);
	size = ftell(fp1);
	rewind(fp1);

	fp2 = fopen(argv[1],"r+");
	if(fp2 == NULL)
	{
		fclose(fp1);
		perror("fopen");
		exit(1);
	}
	count1 = lseek_line_n(fp1,4);
	count2 = lseek_line_n(fp2,3);
	
	fseek(fp1,count1,SEEK_SET);
	fseek(fp2,count2,SEEK_SET);
	
	while(fgets(buf,BUFSIZE,fp1)!= NULL)
	{
		fputs(buf,fp2);
	}	
	
		
	fclose(fp1);
	fclose(fp2);
	
	truncate(argv[1],(size-(count1-count2)));	

	exit(0);
}





