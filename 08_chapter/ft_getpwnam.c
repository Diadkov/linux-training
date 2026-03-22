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

    nobody:*:-2:-2:Unprivileged User:/var/empty:/usr/bin/false
    mtk:x:1000:100:Michael Kerrisk:/home/mtk:/bin/bash
    log name : passwd : userid : groupid : comment : home dir : login shell
    
    root:x:0:0:root:/root:/bin/bash
    open file
    ...
*/
void readLines(int fd, const char* name, void (*processLine)(const char *, const char *))
{
    char buffer[1024];
    char line[2048];
    int lineLen = 0;
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        for (ssize_t i = 0; i < bytesRead; ++i)
        {
            if (buffer[i] == '\n')
            {
                line[lineLen] = '\0';
                processLine(line, name);
                lineLen = 0;
            }
            else
            {
                if (lineLen < (int)sizeof(line) - 1)
                {
                    line[lineLen++] = buffer[i];
                }
                else
                {
                    errExit("Line too long");
                }
            }
        }
    }

    if (bytesRead == -1)
        errExit("read");

    if (lineLen > 0)
    {
        line[lineLen] = '\0';
        processLine(line, name);
    }
}

void processLine(const char *line, const char* name)
{
    if (strncmp(line, name, strlen(name)) == 0)
    {
        printf("%s\n", line);
    }
}

struct passwd *ft_getpwnam(const char* name)
{
    const char* filename = "/etc/passwd";
    int readFlags = O_RDONLY;
    mode_t modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH;
    
    int fd = open(filename, readFlags, modes);
    if(fd == -1)
        errExit("open");

    readLines(fd, name, processLine);
    
    
    // read line
    if(close(fd) == -1)
        errExit("close");

    return NULL;
    // close file
}

int main(int argc, char *argv[])
{
    ft_getpwnam("root");
    
    return 0 ;
}