#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    struct iovec iov[4];
    ssize_t nr;
    int fd, i;
    char *buf[] = {
        "The term buccaneer comes from the word boucan",
        "A boucan is a wooden frame used for cooking meat",
        "Buccaneer is the West Indies name for a pirate",
        "Hello, world"
    };

    fd = open("bucanneer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660);
    if (fd == -1) {
        printf("open error\n");
        return 1;
    }

    for (i = 0; i < 4; i++) {
        iov[i].iov_base = buf[i];
        iov[i].iov_len = strlen(buf[i]);
        printf("%d: %u\n", i, iov[i].iov_len);
        printf("%s\n", iov[i].iov_base);
    }


    nr = writev(fd, iov, 4);
    if (nr == -1) {
        printf("writev error");
        return 1;
    }

    printf("wrote %u bytes\n", nr);

    if (close(fd)) {
        printf("close error\n");
        return 1;
    }

    return 0;
}
