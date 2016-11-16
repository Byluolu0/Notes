#include <fcntl.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
	int accmode, val;
	if (argc != 2) {
		printf("usage: a.out <descriptor#>");
		return -1;
	}

	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
		printf("fcntl error for fd %d", atoi(argv[1]));
		return -1;
	}
	accmode = val & O_ACCMODE;
	if (accmode == O_RDONLY) printf("read only\n");
	else if (accmode == O_WRONLY) printf("write only\n");
	else if (accmode == O_RDWR) printf("read write\n");
	else {
		printf("unknown access mode");
		return -1;
	}

	if (val & O_APPEND) printf("append\n");
	if (val & O_NONBLOCK) printf("nonblocking\n");
	return 0;
}
