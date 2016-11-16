/*
 * 在exec调用以前，子进程在父进程的空间中运行。从结果看出，子进程至少使用了父进程的RW区和栈空间。
 * 结束子进程时使用_exit()而不使用exit()的原因是由于子进程在父进程空间运行，后者会关闭标准输出，导致父进程printf()失效。
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int glob = 6;

int
main()
{
	int var;
	pid_t pid;
	
	var = 88;
	printf("before vfork\n");

	if ( (pid = vfork()) < 0 ) {
		printf("vfork error\n");
		exit(1);
	}
	else if (pid == 0) {
		glob++;
		var++;
		_exit(0);
	}

	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}
