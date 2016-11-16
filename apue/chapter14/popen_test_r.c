#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 2048

int main()
{
    char line[MAXLINE];
    int n;
    while(1) {
        if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0) {
            printf("read error\n");
            return 1;
        }
        else if (n > 0) {
            if (write(STDOUT_FILENO, line, n) < 0) {
                printf("write error\n");
                return 1;
            }
        }
        else
            break;
    }
    exit(0);
}
