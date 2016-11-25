 #define _GNU_SOURCE
       #include <stdio.h>
       #include <stdlib.h>
/*
 *  可以用用ulimite -c 1024,使得内核可以有产生错误现场保存信息的文件
 *	运行./1core 可以产生一个core文件，可以用gdb调试之
 */
       int
       main(void)
       {
           FILE * fp;
           char * line ;
           size_t len ;
           ssize_t read;

           fp = fopen("/etc/services", "r");
           if (fp = NULL)
               exit(EXIT_FAILURE);

           while ((read = getline(&line, &len, fp)) != -1) {
               printf("Retrieved line of length %zu :\n", read);
               printf("%s", line);
           }

           if (line)
               free(line);
           exit(EXIT_SUCCESS);
       }

