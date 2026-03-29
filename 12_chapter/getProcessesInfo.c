#define _GNU_SOURCE
#include "../common/errorFunctions.h"
#include<dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>

/*
Write a program that lists the process ID and command name for all processes
being run by the user named in the program’s command-line argument. (You may
find the userIdFromName() function from Listing 8-1, on page 159, useful.) 

This can be done by inspecting the Name: and Uid: lines of all of the /proc/PID/status files on
the system. Walking through all of the /proc/PID directories on the system requires the
use of readdir(3), which is described in Section 18.8. Make sure your program
correctly handles the possibility that a /proc/PID directory disappears between the
time that the program determines that the directory exists and the time that it tries
to open the corresponding /proc/PID/status file.
*/

uid_t userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    /* Return UID corresponding to 'name', or -1 on error */

    if (name == NULL || *name == '\0')  /* On NULL or empty string */
        return -1;                      /* return an error */

    u = strtol(name, &endptr, 10);

    if (*endptr == '\0')
        return u;   /* As a convenience to caller */
                    /* allow a numeric string */

    pwd = getpwnam(name);

    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

void listFiles(const char* username)
{
    uid_t id = userIdFromName(username);
    if(id == (uid_t)-1)
        errExit("id in listFiles");
    
    DIR* directory = opendir("/proc");
    if(directory == NULL)
        errExit("directory");
    
    struct dirent* mydir = readdir(directory); 
    while(mydir != NULL)
    {   
        char* endptr;
        strtol(mydir->d_name, &endptr, 10);
        
        if(*endptr != '\0')
        {
            mydir = readdir(directory);
            continue;
        }

        char buffer[256];
        int printResult = snprintf(buffer, sizeof(buffer),"/%s/%s/%s", "proc", mydir->d_name, "status");
        
        if(printResult == -1)
            break;

        
        int fd = open(buffer, O_RDONLY);
        char readBuffer[1024];
        if (fd != -1)
        {
            ssize_t n;
            while((n = read(fd, readBuffer, sizeof(readBuffer))) > 0)
            {
                char* substringNameStart = strstr(readBuffer, "Name");
                if(substringNameStart == NULL)
                    break;
                char* substringNameEnd = strchr(substringNameStart, '\n');
                if(substringNameEnd == NULL)
                    break;
                
                *substringNameEnd = '\0';
                printf("%s\n", substringNameStart);
                printf("Pid : %s\n", mydir->d_name);

                *substringNameEnd = '\0';
            }
            printf("--------------------------\n");
        }
            
        close(fd);
        mydir = readdir(directory); 
    }
    
    closedir(directory);
}



#include <stdint.h>
int main(int argc, char *argv[])
{
    if(argc != 2)
        errExit("argc");
    if(strcmp(argv[1], "--help") == 0)
    {
        printf("Usage: ./EXE <nameofuser>\n");
        return EXIT_SUCCESS;    
    }
        
    const char* username = argv[1];
    listFiles(username);
    
    
    
    exit(EXIT_SUCCESS);
}