#include <stdio.h>
#include <fcntl.h>
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main()
{
   int fd;
   if((fd = creat("file.txt", FILE_MODE)) < 0)
   {
       printf("create error\n");
       return -1;
   } 

   int n = 200000, i = 0;
   
   for (i = 0; i < n; i++) {   
   if(write(fd, buf1, 10) != 10)
   {
       printf("buf1 write error\n");
       return -1;
   }
   }
    
   if(lseek(fd, 30, SEEK_SET) == -1)
   {
       printf("lseek error\n");
       return -1;
   }
    
   if(write(fd, buf2, 10) != 10)
   {
       printf("buf2 write error");
       return -1;
   }
    
   return 0;
}
