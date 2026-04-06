#define _GNU_SOURCE
#include "../common/errorFunctions.h"
#include<dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>

/*
The command tail [ –n num ] file prints the last num lines (ten by default) of the
named file. Implement this command using I/O system calls (lseek(), read(), write(),
and so on). Keep in mind the buffering issues described in this chapter, in order to
make the implementation efficient.
*/

void ft_tail(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        errExit("open");

    off_t end = lseek(fd, 0, SEEK_END);
    if (end == -1)
        errExit("lseek");

    off_t pos = end;
    int lines = 0;
    char c;

    while (pos > 0)
    {
        pos--;

        if (lseek(fd, pos, SEEK_SET) == -1)
            errExit("lseek");

        if (read(fd, &c, 1) != 1)
            errExit("read");

        if (c == '\n')
        {
            lines++;
            if (lines == 10)
            {
                pos++; // старт після '\n'
                break;
            }
        }
    }

    if (pos == 0)
    {
        if (lseek(fd, 0, SEEK_SET) == -1)
            errExit("lseek");
    }
    else
    {
        if (lseek(fd, pos, SEEK_SET) == -1)
            errExit("lseek");
    }

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        if (write(1, buffer, bytesRead) != bytesRead)
            errExit("write");
    }

    if (bytesRead == -1)
        errExit("read");

    close(fd);
}

// 0 → stdin
// 1 → stdout
// 2 → stderr
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(2, "Usage: ./tail file\n", 19);
        exit(EXIT_FAILURE);
    }

    ft_tail(argv[1]);
    return 0;
}
