#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <unistd.h>

#include "../common/errorFunctions.h"

/*
Temporary privilege changes can be done by modifying only the effective user ID, 
allowing the process to switch back later. In contrast, setuid() permanently drops 
privileges by also changing the saved user ID, 
making it impossible to regain elevated access.
*/

#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/fsuid.h>

static void print_ids(const char *label)
{
    uid_t realUserId;
    uid_t effectiveUserId;
    uid_t savedUserId;

    if (getresuid(&realUserId, &effectiveUserId, &savedUserId) == -1)
    {
        errExit("getresuid");
        exit(EXIT_FAILURE);
    }

    uid_t fileSystemUserId = setfsuid((uid_t)-1);   

    printf("%s\n", label);
    printf("real=%ld effective=%ld saved=%ld fsuid=%ld\n\n",
           (long)realUserId,
           (long)effectiveUserId,
           (long)savedUserId,
           (long)fileSystemUserId);
}

int main(void)
{
    uid_t x = getuid();
    uid_t y = geteuid();
    print_ids("Before Task 1.");
    if(seteuid(x) == -1)
    {
        errExit("seteuid(x)");
    }
    print_ids("In process of task 1.");
    if(seteuid(y) == -1)
    {
        errExit("seteuid(y)");
    }
    print_ids("End of task 1.");

    print_ids("Before Task 2.");
    if(setuid(x) == -1)
    {
        errExit("seteuid(y)");
    }
    print_ids("End of task 2.");

    return 0;
}