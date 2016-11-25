#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glob.h>
#include <unistd.h>

/*
 * 	功能：用mmap功能，把打开文件映射到内存空间，统计文件中的字符a的个数
 * 	注意:stat lstat fstat 的用法
 *
 *
 */

int main(int argc ,char **argv)
{
	int fd;
	struct stat statres;
	int i,count;

	//mmap返回，以串的形式返回最有利于统计字符 
	char *str;

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}

	fd = open(argv[1],O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	if(fstat(fd,&statres) < 0)
	{
		perror("lstate");
		exit(1);
	}

	str = mmap(NULL,statres.st_size,PROT_READ,MAP_SHARED,fd,SEEK_SET);
	
	
	//有宏名，用宏名，不用宏值 MAP_FAILED == (void *)(-1)
	if(str == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}
	//此处可以关闭fd了，因为fd的内容已经被映射到内存了
	
	close(fd);
	

	for(i = 0 , count = 0; i < statres.st_size ; i++)
	{
		if(str[i] == 'a')
			count ++;
	}

	printf("%d\n",count);

	munmap(str,statres.st_size);
	
	exit(0);
}



