#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/wait.h>
#include <sys/types.h>
/*	实现只是先外部命令的shell
 *	strtok()
 *	strsep()
 */
#define DELIMS	" \t\n" 
struct cmd_st
{
	glob_t glob_res;
};
static void prompt()
{
	printf("mysh-0.1$  ");
}

static parse(char *line,struct cmd_st *res)
{
	char *tok;
	int i = 0;
	while(1)
	{
		tok = strsep(&line,DELIMS);
		if(tok == NULL)
			break;
		if(tok[0] == '\0')
			continue;
		glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL,&res->glob_res);		//注意此处的妙用
		i = 1;
	}		
}

int main()
{
	char *linebuf  = NULL ;
	size_t linebuf_size = 0;
	struct cmd_st cmd;
	pid_t pid;
	while(1)
	{
		prompt(); 			//打印提示符

		if(getline(&linebuf,&linebuf_size,stdin) < 0)			//获取完整的一行
		{
			break;
		}

		parse(linebuf,&cmd);			//解析获取的内容

		if(0)				//如果是内部命令
		{
		
		}else				//如果是外部命令
		{
			pid = fork();
			if(pid < 0)
			{
				perror("fork");
				exit(1);
			}else if(pid == 0)		// 子进程
			{
				execvp(cmd.glob_res.gl_pathv[0],cmd.glob_res.gl_pathv);
				perror("exec()");
				exit(1);
			}else					//父进程
			{
				wait(NULL);
			}
					
		}

	}



	exit(0);
}





