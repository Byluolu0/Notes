#include <unistd.h>

// wait的实现：利用read读管道读端，阻塞
// signal的实现： 利用write写管道写端，消除read的阻塞

static int pfd1[2], pfd2[2];

void
TELL_WAIT()
{
    if (pipe(pdf1) < 0 || pipr(pfd2) < 0) {
        printf("pipe error\n");
        return 1;
    }
}

void
TELL_PARENT(pid_t pid)
{
    if (write(pfd2[1], "c", 1) != 1) {
        printf("write error\n");
        return 1;
    }
}

void WAIT_PARENT()
{
    char c;
    if (read(pfd1[0], &c, 1) != 1) {
        printf("read error\n");
        return 1;
    }
}

void TELL_CHILD(pid_t pid)
{
    if (write(pfd1[1], "p", 1) != 1) {
        printf("write error\n");
        return 1;
    }
}

void WAIT_CHILD()
{
    char c;
    if (read(pfd2[0], &c, 1) != 1) {
        printf("read error\n");
        return 1;
    }
}



