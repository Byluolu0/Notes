#include <stdio.h>
#include <errno.h>
#include <string.h>

int  main()
{
    int i;
    for (i = 0; i < 10; i++) {
        printf("%d: %s\n", i, strerror(i));
    }
    return 0;
}