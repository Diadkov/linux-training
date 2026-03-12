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
    Write a program that opens an existing file for writing with the O_APPEND flag, and
    then seeks to the beginning of the file before writing some data. Where does the
    data appear in the file? Why?
*/

int main(int argc, char *argv[])
{
    int defaultParams = 1;
    if(argc != defaultParams)
        errExit("Parameter arguments Error");

    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int flags = O_WRONLY  | O_CREAT | O_TRUNC | O_APPEND;

    const char* filename = "file.txt";
    int fd = open(filename, flags, perms);
    if(fd == -1)
        errExit("open");

    const char* buffer = "hello\n";
    int bytes = write(fd, buffer, strlen(buffer));
    if(bytes == -1)
        errExit("write");

    lseek(fd, 0, SEEK_SET);
    const char* buffer2 = "Hello2\n"; 
    int bytes2 = write(fd, buffer2, strlen(buffer2));
    if(bytes2 == -1)
        errExit("bytes2");

    if(close(fd) == -1)
        errExit("close file");

    exit(EXIT_SUCCESS);
}