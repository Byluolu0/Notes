#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/types.h>

int
main()
{
	if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
		printf("cannot seek\n");
	else
		printf("seek OK\n");
	exit(0);
}
