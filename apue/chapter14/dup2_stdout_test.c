#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE 1024

int main()
{
    int fd[2], n;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

    printf("hello\n");
    
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    dup2(fd[0], STDOUT_FILENO);
    close(fd[2]);

    exit(0);
    
}
