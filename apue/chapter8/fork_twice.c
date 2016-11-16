/*
 * 第一次fork，在子进程的分支再次fork，然后在第二次fork的父进程分支exit(0);
 * 这样，祖父进程不需要等到孙子进程(实际做事情的进程)，也不会产生僵尸进程。
 *
 * 但是孙子进程getppid()存在竞态条件，如果父进程先运行，exit，打印的是init进程的pid，
 * 如果孙子进程先运行，则打印的是父进程的pid。（孙子进程中sleep就是为了避免这样的情况。）
 * 实际上应该 while(getppid() != 1) sleep(1);
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int
main()
{
	pid_t pid;
	
	if ( (pid = fork()) < 0 ) {
		printf("fork error\n");
		exit(1);
	}
	else if (pid == 0) {
		if ( (pid = fork()) < 0 ) {
			printf("fork error\n");
			exit(1);
		} 
		else if (pid > 0) {
			exit(0);
		}

		sleep(2);
		printf("second child, parent pid = %d\n", getppid());
		exit(0);
	}
	
	// 此pid是第一次fork返回的pid，为子进程id
	if (waitpid(pid, NULL, 0) != pid) {
		printf("waitpid error");
	}

	exit(0);
	
}
