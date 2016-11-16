#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <errno.h>
#include <string.h>

int main()
{
    char foo[45], bar[48], baz[46], hel[12];
    struct iovec iov[4];
    ssize_t nr;
    int fd, i;
                                             
    fd = open("bucanneer.txt", O_RDONLY);
    if (fd == -1) {
        printf("open error\n");
        return 1;
    }

    iov[0].iov_base = foo;
    iov[0].iov_len = sizeof(foo);
    iov[1].iov_base = bar;
    iov[1].iov_len = sizeof(bar);
    iov[2].iov_base = baz;
    iov[2].iov_len = sizeof(baz);
    iov[3].iov_base = hel;
    iov[3].iov_len = sizeof(hel);   

    nr = readv(fd, iov, 4);
    if (nr == -1) {
        printf("readv error\n");
        return 1;
    } 

    for (i = 0; i < 4; i++) {
        printf("%d: %s\n", i, (char *)iov[i].iov_base);
    }

    if (close(fd)) {
        printf("close error\n");
        return 1;
    }

    return 0;
}
