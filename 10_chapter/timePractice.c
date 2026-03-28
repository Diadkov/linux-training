#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/fsuid.h>

#include "../common/errorFunctions.h"

#include<sys/time.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>

#include<sys/times.h>

/*
Assume a system where the value returned by the call sysconf(_SC_CLK_TCK) is
100. 
Assuming that the clock_t value returned by times() is an unsigned 32-bit
integer, how long will it take before this value cycles so that it restarts at 0?

Perform
the same calculation for the CLOCKS_PER_SEC value returned by clock().
*/

void printTime(unsigned long seconds)
{
    unsigned long days = seconds / 86400;
    seconds %= 86400;

    unsigned long hours = seconds / 3600;
    seconds %= 3600;

    unsigned long minutes = seconds / 60;
    seconds %= 60;

    printf("%lu days %lu hours %lu minutes %lu seconds\n",
           days, hours, minutes, seconds);
}

#include <stdint.h>
int main(int argc, char *argv[])
{
    long ticks = sysconf(_SC_CLK_TCK);

    struct tms buf;
    uint32_t timeLimited = times(&buf);
    uint32_t int32max = UINT32_MAX;
    if((clock_t)timeLimited == -1)
        errExit("times()");
    // time till overflow
    printf("max %u\n", int32max);
    printf("limited %u\n", timeLimited);
    printf("Expected to overflow:\n");
    printTime(((int32max - timeLimited) / (uint32_t)ticks));

    printf("\n=== clock() ===\n");

    printf("CLOCKS_PER_SEC %ld\n", (long)CLOCKS_PER_SEC);
    printf("Expected to overflow:\n");

    printTime(((uint32_t)UINT32_MAX / (uint32_t)CLOCKS_PER_SEC));

    
    exit(EXIT_SUCCESS);
}