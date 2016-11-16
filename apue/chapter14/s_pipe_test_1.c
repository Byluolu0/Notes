/*
 * 注意点：1、dup2(fd, STDOUT_FILENO) 后再printf是没有输出的= =
 *         2、write(fd, buf, n)  n要写确实的n
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLINE 1024

static void sig_pipe(int);

int
s_pipe(int fd[2]) 
{
    return (socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}

void err_sys(const char *err_msg)
{
    printf("%s\n", err_msg);
    exit(1);
}

int
main()
{
    int n, fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR)
        err_sys("signal error");

    if (s_pipe(fd) < 0)
        err_sys("s_pipe error");

    if ((pid = fork()) < 0) 
        err_sys("fork error");
    else if (pid > 0) { /* parent */
        close(fd[1]); // 关闭一端
        while(fgets(line, MAXLINE, stdin) != NULL) {
            n = strlen(line);
            line[n] = 0;
            printf("to: %s", line);
            if (write(fd[0], line, n) != n)
                err_sys("write error");
            if ((n = read(fd[0], line, MAXLINE)) < 0)
                err_sys("read error");
            if (n == 0) {
                err_sys("child close pipe");
                break;
            }
            line[n] = 0;
            printf("from: %s\n", line);
        }
        if (ferror(stdin))
            err_sys("fgets error on stdin");
        exit(0);
    } else { /* child */
        close(fd[0]);
        if (fd[1] != STDOUT_FILENO) {
            if(dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) // 复制fd[1],将fd设置为STDOUT_FILENO，然后关闭文件
                err_sys("dup2 error");
        }

        if (fd[1] != STDIN_FILENO) {
            if(dup2(fd[1], STDIN_FILENO) != STDIN_FILENO) // 复制fd[1],将fd置为STDIN_FILENO，然后关闭文件
                err_sys("dup2 error");
        }
        close(fd[1]);
        if (execl("./add2", "add2", NULL) < 0) 
            err_sys("execl error");
    }
}

static void
sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}
