/*
 * 1、如果标准输出连到终端，它是行缓存的，遇到换行符就刷新缓冲区。 
 * 2、但是如果标准输出重定向到一个文件，则它是全缓存的，在进程终止时才会刷新缓冲区。
 * 对于情况1，fork调用时，标准输出缓存里面没有内容，最终看到的结果中，printf的内容只出现一次。
 * 对于情况2，fork调用时，printf的内容还停留在标准输出缓冲区中，也被copy到了子进程，因此最终看到的结果中，printf的内容出现了两次。
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int
main()
{
	int var;
	pid_t pid;
	
	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) { 
		printf("write error");
		exit(1);
	}
	printf("before fork\n");

	if ((pid = fork()) < 0) {
		printf("fork error");
		exit(1);
	}
	else if (pid == 0) {
		glob++;
		var++;
	} else {
//		sleep(2);
	}
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}
