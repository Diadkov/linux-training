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
    Write a program to verify that duplicated file descriptors share a file offset value
    and open file status flags.
*/

int main(int argc, char *argv[])
{   
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int flags = O_WRONLY  | O_CREAT | O_TRUNC;

    const char* filename = argv[1];
    int fd = open(filename, flags, perms);
    if(fd == -1)
        errExit("open: programfile <filename>");

    int dupli1 = dup(fd);
    int dupli2 = dup(fd);
    if(dupli1 == -1)
        errExit("dupli1");
    if(dupli2 == -1)
        errExit("dupli2");

    const char* buffer = "AAA";
    ssize_t bytesWritten = write(fd, buffer, strlen(buffer));
    if(bytesWritten == -1)
        errExit("write");
    off_t offset1 = lseek(dupli1, 0, SEEK_CUR);
    off_t offset2 = lseek(dupli2, 0, SEEK_CUR);

    if(offset1 == -1)
        errExit("offset1");
    if(offset2 == -1)
        errExit("offset2");

    printf("offset dupli1 = %ld\n", (long) offset1);
    printf("offset dupli2 = %ld\n", (long) offset2);


    if(offset1 == offset2)
        printf("offsets are equal\n");
    else
        errExit("offsets");

    int flagsDupli1 = fcntl(dupli1, F_GETFL);
    int flagsDupli2 = fcntl(dupli2, F_GETFL);
    
    if(flagsDupli1 == -1)
        errExit("flags duplicate 1");
    if(flagsDupli2 == -1)
        errExit("flags duplicate 2");
    
    if (flagsDupli1 == flagsDupli2)
        printf("flags are equal\n");
    else     
        errExit("fcntl");
    
    if (close(fd) == -1)
    errExit("close fd");

    if (close(dupli1) == -1)
        errExit("close dupli1");

    if (close(dupli2) == -1)
        errExit("close dupli2");

    printf("Operation successful\n");
    exit(EXIT_SUCCESS);
}