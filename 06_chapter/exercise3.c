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

/*
    Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary,
    code that directly modifies environ. Your version of unsetenv() should check to see
    whether there are multiple definitions of an environment variable, and remove
    them all (which is what the glibc version of unsetenv() does).
*/

// setenv
// unsetenv

//getenv(), putenv()

/*
char *getenv(const char *name);

Returns pointer to (value) string, or NULL if no such variable
*/


// extern char **environ;
// int main(int argc, char *argv[])
// {
//     char **ep;
//     for (ep = environ; *ep != NULL; ep++)
//     puts(*ep);
//     exit(EXIT_SUCCESS);
// }




int ft_setenv(const char *name, const char *value, int overwrite)
{
    if(getenv(name) != NULL && overwrite == 0)
        return 0;
    size_t length = strlen(name) + strlen(value) + 2; // = and \0
    char* env = malloc(length);
    if(env == NULL)
        return -1;
    int bytes = snprintf(env, length, "%s=%s", name, value);
    
    if(bytes == -1)
    {
        free(env);
        return -1;
    }
    
    if (putenv(env) != 0)
    {
        free(env);
        return -1;
    }

    return 0;
}

// used in printEnvironment and ft_unsetenv
extern char **environ;

int ft_unsetenv(const char* name)
{
    size_t lengthOfArray = 1024;
    char* environmentInfo = getenv(name);

    if(environmentInfo == NULL)
        return 0;
    if (!name || name[0] == '\0' || strchr(name, '='))
        return -1;

    char **ep;
    for (ep = environ; *ep != NULL; ep++)
    {
        char* equation = strchr(*ep, '=');
        if(!equation)
            continue;

        size_t length = equation - *ep;
        char nameOfEnvironment[lengthOfArray];

        memcpy(nameOfEnvironment, *ep,length);
        nameOfEnvironment[length]= '\0'; // need to do that because of memcpy specific - 

        if(strcmp(nameOfEnvironment, name) == 0)
        {
            char** next = ep;
            while(*next)
            {
                *next = *(next + 1);
                ++next;
            }   
            --ep; // get on the prev element
        }
    }
    return 0;
}

void printEnvironment()
{
    printf("------------------------------------------------\n");
    char **ep;
    for (ep = environ; *ep != NULL; ep++)
    {
        puts(*ep);
    }
    printf("------------------------------------------------\n");
}
int main(int argc, char *argv[])
{   
    ft_setenv("HELLO_WORLD", "AIAI", 0);
    printEnvironment();
    ft_unsetenv("HELLO_WORLD");
    printEnvironment();
    
    printf("Operation successful\n");

    exit(EXIT_SUCCESS);
}