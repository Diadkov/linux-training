#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <stdarg.h>
#include "../common/errorFunctions.h"

/*
After each of the calls to write() in the following code, explain what the content of
the output file would be, and why:

fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
fd2 = dup(fd1);
fd3 = open(file, O_RDWR);
write(fd1, "Hello,", 6);
write(fd2, "world", 6);

lseek(fd2, 0, SEEK_SET);
write(fd1, "HELLO,", 6);
write(fd3, "Gidday", 6);
*/

int main(int argc, char *argv[])
{   
    if(argc != 2)
        errExit("argc");
    const char* file = argv[1];
    int fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); // no append - not atomic
    int fd2 = dup(fd1); // dupllicate
    int fd3 = open(file, O_RDWR); // opening same file with another access and offset on the beginnning of file 
    write(fd1, "Hello,", 6); // Hello,
    write(fd2, "world", 6); // Hello,world\0 - is in file 

    lseek(fd2, 0, SEEK_SET); // changes offset for BOTH fd1 and fd2 (shared offset)
    write(fd1, "HELLO,", 6); // HELLO,world\0 - in file
    write(fd3, "Gidday", 6); // Giddayworld\0

    printf("Operation successful\n");
    exit(EXIT_SUCCESS);
}