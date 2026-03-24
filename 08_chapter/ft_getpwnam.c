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
    Implement getpwnam() using setpwent(), getpwent(), and endpwent().
    
    The getpwnam() function returns a pointer to a structure
    containing the broken-out fields of the record in the password
    database (e.g., the local password file /etc/passwd, NIS, and
    LDAP) that matches the username name.

               struct passwd {
               char   *pw_name;       username 
               char   *pw_passwd;     user password
               uid_t   pw_uid;        user ID 
               gid_t   pw_gid;        group ID 
               char   *pw_gecos;      user information 
               char   *pw_dir;        home directory 
               char   *pw_shell;      shell program 
           };
*/

#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void free_passwd(struct passwd *p)
{
    if (!p) return;

    free(p->pw_name);
    free(p->pw_passwd);
    free(p->pw_gecos);
    free(p->pw_dir);
    free(p->pw_shell);
    free(p);
}

static char *safe_strdup(const char *src)
{
    if (!src) return NULL;

    char *copy = strdup(src);
    if (!copy)
        return NULL;

    return copy;
}

static struct passwd *copy_passwd(struct passwd *src)
{
    struct passwd *dst = malloc(sizeof(struct passwd));
    if (!dst)
        return NULL;

    dst->pw_uid = src->pw_uid;
    dst->pw_gid = src->pw_gid;

    dst->pw_name   = safe_strdup(src->pw_name);
    dst->pw_passwd = safe_strdup(src->pw_passwd);
    dst->pw_gecos  = safe_strdup(src->pw_gecos);
    dst->pw_dir    = safe_strdup(src->pw_dir);
    dst->pw_shell  = safe_strdup(src->pw_shell);

    if (!dst->pw_name || !dst->pw_passwd || !dst->pw_gecos ||
        !dst->pw_dir  || !dst->pw_shell)
    {
        free_passwd(dst);
        return NULL;
    }

    return dst;
}

struct passwd *ft_getpwnam(const char *name)
{
    struct passwd *entry;

    if (!name)
        return NULL;

    setpwent();

    while ((entry = getpwent()) != NULL)
    {
        if (strcmp(entry->pw_name, name) == 0)
        {
            struct passwd *result = copy_passwd(entry);
            endpwent();
            return result;
        }
    }

    endpwent();
    return NULL;
}

int main(void)
{
    struct passwd *p = ft_getpwnam("root");

    if (p)
    {
        printf("User: %s\n", p->pw_name);
        printf("UID: %d\n", p->pw_uid);
        printf("Home: %s\n", p->pw_dir);
        printf("Shell: %s\n", p->pw_shell);

        free_passwd(p);
    }
    else
    {
        printf("User not found\n");
    }

    return 0;
}