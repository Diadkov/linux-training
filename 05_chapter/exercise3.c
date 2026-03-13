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

This exercise is designed to demonstrate why the atomicity guaranteed by opening
a file with the O_APPEND flag is necessary. Write a program that takes up to three
command-line arguments:

    $ atomic_append filename num-bytes [x]
    
This file should open the specified filename (creating it if necessary) and append
num-bytes bytes to the file by using write() to write a byte at a time. By default, the
program should open the file with the O_APPEND flag, but if a third command-line
argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the
program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run
two instances of this program at the same time without the x argument to write
1 million bytes to the same file:

    $ atomic_append f1 1000000 & atomic_append f1 1000000

Repeat the same steps, writing to a different file, but this time specifying the x
argument:

    $ atomic_append f2 1000000 x & atomic_append f2 1000000 x
List the sizes of the files f1 and f2 using ls –l and explain the difference.
*/

void writeBytes(int fd, int numOfBytes, int useSeek)
{
    const char letter = 'A';

    for (int i = 0; i < numOfBytes; ++i) {

        if (useSeek)
            lseek(fd, 0, SEEK_END);

        if (write(fd, &letter, 1) == -1)
            errExit("write");
    }
}

int main(int argc, char *argv[])
{
    // atomic_append filename num-bytes [x]
    
    int numsWithdefaultParams = 3, numsWithAdditionalParams = 4;

    if(argc != numsWithdefaultParams && argc != numsWithAdditionalParams)
        errExit("Error num of params");

    const char* command = "atomic_append"; // filename - argv[1]

    if(strcmp(argv[1], command) != 0)
        errExit("Parameter arg [%s] not equal to actual  [%s]", argv[1], command);
    
    int numOfBytes;
    char* end;
    int base = 10;
    numOfBytes = strtol(argv[2], &end, base);

    if(numOfBytes < 0)
        errExit("negative number of bits");

    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int flags = O_WRONLY  | O_CREAT;

    const char* filename = "file.txt";
    int fd = open(filename, flags, perms);
    if(fd == -1)
        errExit("open");
    
    if(numsWithdefaultParams == argc)
    {
        int allFlags = fcntl(fd, F_GETFL);   
        if (allFlags == -1)
            errExit("fcntl");

        allFlags |= O_APPEND;

        if (fcntl(fd, F_SETFL, allFlags) == -1)
            errExit("fcntl");
    }

    int withAppend = 0, withoutAppend = 1;
    int useSeek = withoutAppend;

    if((fcntl(fd, F_GETFL) & O_APPEND) != 0) // if value is set
        useSeek = withAppend;

    writeBytes(fd, numOfBytes, useSeek);

    if(close(fd) == -1)
        errExit("close file");

    printf("Operation successful\n");
    exit(EXIT_SUCCESS);
}