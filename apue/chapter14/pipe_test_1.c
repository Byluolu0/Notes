#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 2048

int
main()
{
    int n, fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (pipe(fd) < 0) {
        printf("pipe error\n");
        return 1;
    }
    
    if ((pid = fork()) < 0) {
        printf("fork error\n");
        return 1;
    }
    else if (pid > 0) { // 父进程，关闭读端，往写端写数据
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    } else { // 子进程，关闭写端，从读端读取数据
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
        //printf("%s\n", line);
    }
    exit(0);
}
