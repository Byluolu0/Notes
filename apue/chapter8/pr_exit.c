#include "pr_exit.h"

int 
main()
{
	int zero = 0;
	pid_t pid, wpid;
	int status;
	
	if ( (pid = fork()) < 0 ) {
		printf("fork error\n");
		exit(1);
	}
	else if ( pid == 0 ) {
		exit(7);
	}

	if ((wpid = wait(&status)) != pid) {
		printf("wait error %d %d\n", wpid, pid);
		exit(1);
	}
	
	pr_exit(status);
	
	if ((pid = fork()) < 0) {
		printf("fork error\n");
		exit(1);
	} else if ( pid == 0 ) {
		abort(); /* signal */
	}

	if (wait(&status) != pid) {
		printf("wait error\n");
		exit(1);
	}
	pr_exit(status);

	if ((pid = fork()) < 0) {
		printf("fork error\n");
		exit(1);
	} else if (pid == 0) {
		status /= zero;
	}

	if (wait(&status) != pid) {
		printf("wait error\n");
		exit(1);
	}
	pr_exit(status);
	return 0;
}
