#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <stdarg.h>
#include "../common/errorFunctions.h"
#include <setjmp.h>

static jmp_buf env;

int broken()
{ 
    int variableToAccess = -1;
    printf("my variable: %d\n", variableToAccess);
    setjmp(env);

    return variableToAccess; // return from old stack pointer
}

int main(int argc, char *argv[])
{   
    int value = broken();
    printf("Value from broken %d\n", value);
    longjmp(env, 1);

    printf("Operation successful\n");
    exit(EXIT_SUCCESS);
}