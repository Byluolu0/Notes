#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#define BUFFSIZE 4000
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int
main()
{
	int n;
	int val;
	char buf[BUFFSIZE];
	int w_fd;
	
	w_fd = open("out.txt", O_WRONLY | O_SYNC, FILE_MODE);
	while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(w_fd, buf, n) != n) {
			printf("write error");
			return -1;
		}
	}

	if (n < 0) {
		printf("read error");
		return -1;
	}

	return 0;
}

