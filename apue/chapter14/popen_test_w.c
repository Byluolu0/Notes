/*
 * popen()函数： 先fork，然后exec，返回一个FILE*。如果是以"w"打开，那么返回的是exec函数的标准输入，是管道的写端。
 * 如果是以"r"打开，那么返回的是exec函数的标准输出，是管道的读端。
 */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include "apue_popen.h"

#define MAXLINE 2048

int
main(int argc, char *argv[]) 
{
    char line[MAXLINE];
    FILE *fpin, *fpout;

    if (argc < 3) {
        printf("usage: <exename> <pathname> <outexe>\n");
        return 1;
    }

    if ((fpin = fopen(argv[1], "r")) == NULL) {
        printf("fopen error\n");
        return 1;
    }

    if ((fpout = popen(argv[2], "w")) == NULL) {
        printf("popen error\n");
        return 1;
    }

    while (fgets(line, MAXLINE, fpin) != NULL) {
        if (fputs(line, fpout) == EOF) {
            printf("fputs error to pipe\n");
            return 1;
        }
    }    

    if (pclose(fpout) == -1) {
        printf("pclose error\n");
        return 1;
    }

    exit(0);
}
