#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void
set_fl(int fd, int flags)
{
	int val;
	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		printf("fcntl error\n");
		exit(1);
	}

	val |= flags;

	if (fcntl(fd, F_SETFL, val) < 0) {
		printf("fcntl error\n");
		exit(1);
	}
}

int main()
{
	int val;
	int fd;
	fd = open("out.txt", O_WRONLY, FILE_MODE);
	set_fl(fd, O_SYNC);
	if((val = fcntl(fd, F_GETFL, 0)) < 0) {
		printf("fcntl error\n");	
		exit(1);
	}	
	if ( val & O_SYNC) {
		printf("is sync\n");
	}
	else {
		printf("not sync\n");
	}
	return 0;
}
