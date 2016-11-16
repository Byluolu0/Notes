#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define N 5000

static void charatatime(char *);
char s[N];

int
main()
{
	pid_t pid;
	
	if ( (pid = fork()) < 0 ) {
		printf("fork error\n");
		exit(1);
	}
	else if (pid == 0) {
		memset(s, 's', sizeof(s));
		charatatime(s);
	} else {
		// 这个是等待子进程终止，跟信号不一样
		// waitpid(pid);
		memset(s, 'p', sizeof(s));
		charatatime(s);
	}

	exit(0);
}

static void
charatatime(char *str) 
{
	char *ptr;
	int c;
	
	setbuf(stdout, NULL);
	for (ptr = str; c = *ptr++; )
		putc(c, stdout);
}


