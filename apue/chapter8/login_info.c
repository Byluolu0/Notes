#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char *loginInfo = getlogin();
	printf("%s\n", loginInfo);
	exit(0);
}
