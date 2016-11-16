#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAXLINE 1024

int
main()
{
    int n, int1, int2;
    char line[MAXLINE];
    char log[MAXLINE];

    FILE *fp = fopen("/home/ubuntu/apue/chapter14/temp.txt", "r+");    

    while((n = read(STDIN_FILENO, line, MAXLINE)) > 0) {
        line[n] = 0;
        if (sscanf(line, "%d%d", &int1, &int2) == 2) {
            sprintf(line, "%d", int1 + int2);
            n = strlen(line);
            if (write(STDOUT_FILENO, line, n) != n) {
                return 1;
            } 
        } else {
            if (write(STDOUT_FILENO, "invalid args\n", 13) != 13) {
                write(fileno(fp), "write error1\n", 14);
                return 1;
            }
        }
    }
    exit(0);
}
