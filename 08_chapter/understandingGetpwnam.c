#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>

#include "../common/errorFunctions.h"

/*
When we execute the following code, we find that it displays the same number
twice, even though the two users have different IDs in the password file. Why is this?
printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid),
(long) (getpwnam("tsr")->pw_uid));
*/

// this example shows us that we getpwnam - uses static buffer that's why we can get a == b -> true
// solution 1:  use getpwnam and cop all the variables than use another one
// solution 2: use getpwnam_r

int main(int argc, char *argv[])
{
    struct passwd *a = getpwnam("root");
    struct passwd *b = getpwnam("daemon");

    printf("%ld %ld\n", (long)a->pw_uid, (long)b->pw_uid);
    
    return 0;
}