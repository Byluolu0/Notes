#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

static void sig_pipe(int);

int
main()
{
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
        printf("signal error\n");
        return 1;
    }

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("pipe error\n");
        return 1;
    }

    if ((pid = fork()) < 0) {
        printf("fork error\n");
        return 1;
    }
    else if (pid > 0) { // 父进程
        close(fd1[0]); // 关闭读端，用去输入
        close(fd2[1]); // 关闭写端，用于接收输出
        while (fgets(line, MAXLINE, stdin) != NULL) { // 从标准输入读数据
            n = strlen(line);
            if (write(fd1[1], line, n) != n) { // 写到管道写端，子进程在对应的读端接收
                printf("write error\n");
                return 1;
            }

            if ((n = read(fd2[0], line, MAXLINE)) < 0) { // 从管道读端接收数据，该数据为子进程处理完后写到 对应管道的写端的
                printf("read error\n");
                return 1;
            }
    
            if (n == 0) {
                printf("child close pipe\n");
                break;
            }

            line[n] = 0;
            if (fputs(line, stdout) == EOF) { // 把数据写到标准输出
                printf("fputs error");
                return 1;
            }
        }
        if (ferror(stdin)) {
            printf("fget error on stdin\n");
            return 1;    
        }
        exit(0);
    } else { // 子进程
        close(fd1[1]); // 关闭写端，从fd1读端读数据
        close(fd2[0]); // 关闭读端，对fd2写端写数据
        if (fd1[0] != STDIN_FILENO) {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) { //复制fd1[0]文件描述符，并且fd制定为STDIN_FILENO，然后关闭原描述符
                printf("dup2 error\n");
                return 1;
            }
            close(fd1[0]);
        }
        
        if (fd2[1] != STDOUT_FILENO) {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) { // 复制fd2[1]文件描述符，指定fd为STDOUT_FILENO,然后关闭原描述符
                printf("dup2 error\n");
                return 1;
            }
            close(fd2[1]);
        }
        if (execl("./add2", "add2", (char *)0)) {
            printf("execl error\n");
            return 1;
        }
    }
    exit(0);
}

static void
sig_pipe(int signo) 
{
    printf("SIGPIPE caught\n");
    exit(1);
} 

