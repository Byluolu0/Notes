#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    struct stat sb;
    off_t len;
    char *p;
    int fd;
    if (argc < 2) {
        printf("usage: %s <file>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("open error\n");
        return 1;
    }

    if (fstat(fd, &sb) == -1) {
        printf("fstat error\n");
        return 1;
    }
    
    if (!S_ISREG(sb.st_mode)) {
        printf("%s is not a file\n", argv[1]);
        return 1;
    }
    
    /*
     * 如果成功，直接通过p[x]访问
     */
    p = mmap(0, sb.st_size, PROT_READ,
        MAP_SHARED, fd, 0);

    if (p == MAP_FAILED) {
        printf("mmap error\n");
        return 1;
    }

    if (close(fd) == -1) {
        printf("close error\n");
        return 1;
    }

    /*
     * putchar()直接写到标准输出
     */
    for (len = 0; len < sb.st_size; len++) {
        putchar(p[len]);
    }

    if (munmap(p, sb.st_size) == -1) {
        printf("munmap error\n");
        return 1;
    }

    return 0;
}
