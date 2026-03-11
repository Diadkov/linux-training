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

void generateFileWithHoles(const char* filename)
{
    int errorValue = -1;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    
    int BUF_SIZE_LOCAL = 2048;
    char buffer[BUF_SIZE_LOCAL];

    char letter = 'Y';

    for(int i = 0, newlineIndex = 40; i < BUF_SIZE_LOCAL; ++i)
    {
        if(i % newlineIndex == newlineIndex - 1)
            buffer[i] = '\n';
        else
            buffer[i] = letter;
    }
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, filePerms);
    if(fd == errorValue)
        errExit("Open");

    int bytesWritten = write(fd, buffer, BUF_SIZE_LOCAL);
    if(bytesWritten == errorValue)
        errExit("Error reading from buffer");

    off_t offset = lseek(fd, 100, SEEK_CUR);
    if(offset == errorValue)
        errExit("Error reading from buffer");

    bytesWritten = write(fd, buffer, BUF_SIZE_LOCAL);
    if(bytesWritten == errorValue)
        errExit("Error reading from buffer");

    close(fd);
}

int main(int argc, char *argv[])
{
    int defaultParams = 4;
    if(argc != defaultParams)
        errExit("Parameter arguments");
    int errorValue = -1;

    const char* copy = argv[1];
    int stringsEqual = 0;
    if(strcmp(copy, "cp") != stringsEqual)
        errExit("%s, Command to start copying: <cp> <file> <file>", copy);

    const char* fromFile = argv[2];
    const char* toFile = argv[3];
    
    generateFileWithHoles(fromFile);
    
    int readFlags = O_RDONLY;
    int writeFlags = O_WRONLY  | O_CREAT | O_TRUNC;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH;
    
    int fdFrom = open(fromFile, readFlags);
    if(fdFrom == errorValue)
        errExit("Error reading input file");

    int fdTo = open(toFile, writeFlags, filePerms);
    if(fdTo == errorValue)
        errExit("Error reading output file");
    
    off_t size = lseek(fdFrom, 0, SEEK_END);
    off_t offset = lseek(fdFrom, 0, SEEK_SET);
    
    const int lineLength = 1024;

    while(offset < size)
    {
        off_t offsetData = lseek(fdFrom, offset, SEEK_DATA);
        if(offsetData == errorValue)
            offsetData = size;
        lseek(fdFrom, offset, SEEK_SET);
        
        off_t offsetHole = lseek(fdFrom, offset, SEEK_HOLE);
        if(offsetHole == errorValue)
            offsetHole = size;
        lseek(fdFrom, offset, SEEK_SET);

        char buffer[lineLength];

        if(offset < offsetData)
        {
            offset = offsetData;

            if(lseek(fdFrom, offset, SEEK_SET) == errorValue)
                errExit("lseek fdFrom");

            if(lseek(fdTo, offset, SEEK_SET) == errorValue)
                errExit("lseek fdTo");

            if(offset == errorValue)
                errExit("Error processing offset");
        }
        else
        {
            ssize_t bytesForOffset = offsetHole - offset;
            while(bytesForOffset > 0)
            {
                ssize_t toRead = bytesForOffset > sizeof(buffer) ? sizeof(buffer) : bytesForOffset;
                ssize_t bytesRead = read(fdFrom, buffer, toRead);
                if(bytesRead == errorValue)
                    errExit("Reading error");
                offset += bytesRead;

                ssize_t bytesWritten = write(fdTo, buffer, bytesRead);
                if(bytesWritten == errorValue)
                    errExit("Error with writing");

                if(bytesRead != bytesWritten)
                    fatal("Error read-write file");
                bytesForOffset -= bytesRead;
            }
        }
    }

    if(close(fdFrom) == errorValue)
        errExit("Error closing input file");
    if(close(fdTo) == errorValue)
        errExit("Error closing output file");

    printf("Sucesfully completed...\n");
    
    exit(EXIT_SUCCESS);
}