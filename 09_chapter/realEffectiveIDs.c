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

/*
Assume in each of the following cases that the initial set of process user IDs
is real=1000 effective=0 saved=0 file-system=0. What would be the state of the user
IDs after the following calls? 
a) setuid(2000); 
b) setreuid(–1, 2000); 
c) seteuid(2000); 
d) setfsuid(2000); 
e) setresuid(–1, 2000, 3000);


for reset of all commands use this :
sudo chown root:root differentIDs 
sudo chmod u+s differentIDs 

*/

static void print_ids(const char *label)
{
    uid_t realUserId;
    uid_t effectiveUserId;
    uid_t savedUserId;

    if (getresuid(&realUserId, &effectiveUserId, &savedUserId) == -1)
    {
        perror("getresuid");
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
    // setuid(2000) -> if the process is privileged, real, effective,
    // saved, and fsuid become 2000

    // setreuid(-1, 2000) -> real stays unchanged, effective becomes 2000
    // saved may also change to 2000

    // seteuid(2000) -> changes only effective UID
    // saved stays unchanged, so you can later return to root

    // setfsuid(2000) -> changes only file-system UID

    // setresuid(-1, 2000, 3000) -> real stays unchanged,
    // effective becomes 2000, saved becomes 3000,
    // fsuid becomes 2000
    print_ids("before setuid(2000):");
    errno = 0;
    if (setresuid(-1, 2000, 3000) == -1)
        printf("setuid failed: %s\n\n", strerror(errno));
    print_ids("after setuid(2000):");

    return 0;
}