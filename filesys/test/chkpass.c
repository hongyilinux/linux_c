#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>


int main(int argc ,char **argv)
{

	if(argc < 2)
	{
		fprintf(stderr,"Usage...\n");
		exit(1);
	}
	
	//security
	// 需要注意数据安全，隐藏终端输入

	char *input_pass;
	char *crypted_pass;
	struct spwd *shdowline;	
	input_pass = getpass("Passwd:");//用来做口令输入	
	
	shdowline = getspnam(argv[1]);  //需要校验，因为，可能没有权限，导致失败的
	
	crypted_pass = crypt(input_pass,shdowline->sp_pwdp);  //只能看见三个$前的内容，后面的看不见，所以不截断字符串
	
	
	if(strcmp(crypted_pass,shdowline->sp_pwdp) == 0)
	{
		printf("succss\n");		
	}else
		printf("failed\n");
	
	
	exit(0);
}









