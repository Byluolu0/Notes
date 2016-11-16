#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int
main()
{
	pid_t pid;
	if ((pid = fork()) < 0) {
		printf("fork error");
		exit(1);
	}
	else if(pid == 0) {
		if (execl("/home/ubuntu/apue/chapter8/testinterp", "arg1", "arg2", (char *)0) < 0) {
			printf("execl error");
			exit(1);
		}
	}

	if (waitpid(pid, NULL, 0) < 0) {
		printf("waitpid error");
		exit(1);
	}
	exit(0);
}
