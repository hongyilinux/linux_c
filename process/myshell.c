#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>

#define		DELIMS	" \t\n"
/*
 *	需要用到glob_t 结构体
 *
 *
 *
 *
 */
typedef struct 
{
	glob_t globres;
}cmd_st;

static void propmt(void)
{
	printf("mysh-0.1$ ");
}

static void  pares(char *line,cmd_st *res)
{
	int i = 0 ;	
	char *tok;

	while(1)
	{
		tok = strsep(&line,DELIMS);
		if(tok == NULL)
			break;
		if(tok[0] == '\0') //表明解析到空串
			continue;
		glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&(res->globres));
		i = 1;
	
	}
}


int main()
{
	pid_t pid;
	char *linebuf = NULL;
	size_t linebuf_size = 0;
	cmd_st res;
	while(1)
	{

		//打印提示符
		propmt();
		//getline(); 拿到完整一行
		
		if(getline(&linebuf,&linebuf_size,stdin) < 0)
			break;
		
		//pares（）；解析，区分外部命令还是内部命令
		
		pares(linebuf,&res);

		if(0)   //内部命令暂时不做处理
		{}
		else		//处理外部命令
		{
			pid = fork();
			if(pid < 0)
			{
				perror("fork");
				exit(1);
			}
			if(pid == 0)
			{
				execvp(res.globres.gl_pathv[0],res.globres.gl_pathv);
				perror("execvp()");
				exit(1);
			}
			else
			{
				wait(NULL);
			}			
		}



	}


	exit(0);
}
