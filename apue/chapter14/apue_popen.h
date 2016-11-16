#include <stdio.h>
#include <unistd.h>
#include <errno.h>

static pid_t *childpid = NULL;

static int maxfd = 1024;

#define SHELL "/bin/sh"

FILE *
popen(const char *cmdstring, const char *type)
{
    int i, pfd[2];
    pid_t pid;
    FILE *fp;

    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
        printf("param type for popen error\n");
        return NULL;
    }

    if (childpid == NULL) { /* first time */
        //maxfd = open_max();
        if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL) {
            printf("calloc error\n");
            return NULL;
        }
    }

    if (pipe(pfd) < 0) {
        printf("pipe error\n");
        return NULL;
    }

    if ((pid = fork()) < 0) {
        printf("fork error\n");
        return NULL;
    }
    else if (pid == 0) {
        if (*type == 'r') { /* fopen() for read */
            close(pfd[0]); /* close read end */
            if (pfd[1] != STDOUT_FILENO) {
                dup2(pfd[1], STDOUT_FILENO); /* copy STDOUT_FILENO as pipe write end  */
                close(pfd[1]); /* close the pre write end */
            }
        } else if (*type == 'w') {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO) {
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }

        for (i = 0; i < maxfd; i++) { /* 父进程打开的所有文件，子进程也共享，所以要手动关闭 */
            if (childpid[i] > 0) {
                close(i);
            }
        }
        execl(SHELL, "sh", "-c", cmdstring, (char *) 0);
        _exit(127);
    }
    
    /* parent */
    if (*type == 'r') {
        close(pfd[1]);
        if ((fp = fdopen(pfd[0], type)) == NULL) { /* 字面意思，标准I/O，通过打开的fd来打开文件，返回FILE指针 */
            printf("fdopen error\n");
            return NULL;
        }
    } else {
        close(pfd[0]);
        if ((fp = fdopen(pfd[1], type)) == NULL) {
            printf("fdopen error\n");
            return NULL;
        }
    }
    childpid[fileno(fp)] = pid;
    return fp;
}

int
pclose(FILE *fp)
{
    int fd, stat;
    pid_t pid;
    if (childpid == NULL)
        return -1;

    fd = fileno(fp);
    if ((pid = childpid[fd]) == 0) {
        return -1;
    }

    childpid[fd] = 0;
    if (fclose(fp) == EOF)
        return -1;

    while (waitpid(pid, &stat, 0) < 0) {
        if (errno != EINTR) {
            return -1;
        }
    }
    return stat;
}
