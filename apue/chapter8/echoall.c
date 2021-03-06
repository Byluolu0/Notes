#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int
main(int argc, char *argv[])
{
	int i;
	char **ptr;

	for (i = 0; i < argc; i++) 
		printf("argv[%d]: %s\n", i, argv[i]);

	for (ptr = environ; *ptr != 0; ptr++)
		printf("%s\n", *ptr);

	exit(0);
}
