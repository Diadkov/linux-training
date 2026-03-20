#define _GNU_SOURCE
#include "errorFunctions.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
\*************************************************************************/

/* Listing 3-3 */

/* error_functions.c

   Some standard error handling routines used by various programs.
*/

const char *ename[] = {
    "",
    "EPERM",
    "ENOENT",
    "ESRCH",
    "EINTR",
    "EIO",
    "ENXIO",
    "E2BIG",
    "ENOEXEC",
    "EBADF",
    "ECHILD",
    "EAGAIN",
    "ENOMEM",
    "EACCES",
    "EFAULT"
};

#define MAX_ENAME 14

#ifdef __GNUC__                 /* Prevent 'gcc -Wall' complaining  */
__attribute__ ((__noreturn__))  /* if we call this function as last */
#endif                          /* statement in a non-void function */
static void
terminate(Boolean useExit3)
{
    char *s;

    /* Dump core if EF_DUMPCORE environment variable is defined and
       is a nonempty string; otherwise call exit(3) or _exit(2),
       depending on the value of 'useExit3'. */

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/* Diagnose 'errno' error by:

      * outputting a string containing the error name (if available
        in 'ename' array) corresponding to the value in 'err', along
        with the corresponding error message from strerror(), and

      * outputting the caller-supplied error message specified in
        'format' and 'ap'. */

static void
outputError(Boolean useErr, int err, Boolean flushStdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);
#pragma GCC diagnostic pop

    if (flushStdout)
        fflush(stdout);       /* Flush any pending stdout */
    fputs(buf, stderr);
    fflush(stderr);           /* In case stderr is not line-buffered */
}

/* Display error message including 'errno' diagnostic, and
   return to caller */

void
errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno;

    savedErrno = errno;       /* In case we change it here */

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    errno = savedErrno;
}

/* Display error message including 'errno' diagnostic, and
   terminate the process */

void
errExit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/* Display error message including 'errno' diagnostic, and
   terminate the process by calling _exit().

   The relationship between this function and errExit() is analogous
   to that between _exit(2) and exit(3): unlike errExit(), this
   function does not flush stdout and calls _exit(2) to terminate the
   process (rather than exit(3), which would cause exit handlers to be
   invoked). */

void
err_exit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errno, FALSE, format, argList);
    va_end(argList);

    terminate(FALSE);
}

/* The following function does the same as errExit(), but expects
   the error number in 'errnum' */

void
errExitEN(int errnum, const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(TRUE, errnum, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/* Print an error message (without an 'errno' diagnostic) */

void
fatal(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(FALSE, 0, TRUE, format, argList);
    va_end(argList);

    terminate(TRUE);
}

/* Print a command usage error message and terminate the process */

void
usageErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);           /* Flush any pending stdout */

    fprintf(stderr, "Usage: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);           /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}

/* Diagnose an error in command-line arguments and
   terminate the process */

void
cmdLineErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);           /* Flush any pending stdout */

    fprintf(stderr, "Command-line usage error: ");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);           /* In case stderr is not line-buffered */
    exit(EXIT_FAILURE);
}

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
/* Print a diagnostic message that contains a function name ('fname'),
   the value of a command-line argument ('arg'), the name of that
   command-line argument ('name'), and a diagnostic error message ('msg'). */

static void
gnFail(const char *fname, const char *msg, const char *arg, const char *name)
{
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        offending text: %s\n", arg);

    exit(EXIT_FAILURE);
}

/* Convert a numeric command-line argument ('arg') into a long integer,
   returned as the function result. 'flags' is a bit mask of flags controlling
   how the conversion is done and what diagnostic checks are performed on the
   numeric result; see get_num.h for details.

   'fname' is the name of our caller, and 'name' is the name associated with
   the command-line argument 'arg'. 'fname' and 'name' are used to print a
   diagnostic message in case an error is detected when processing 'arg'. */

static long
getNum(const char *fname, const char *arg, int flags, const char *name)
{
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
                        (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gnFail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}

/* Convert a numeric command-line argument string to a long integer. See the
   comments for getNum() for a description of the arguments to this function. */

long
getLong(const char *arg, int flags, const char *name)
{
    return getNum("getLong", arg, flags, name);
}

/* Convert a numeric command-line argument string to an integer. See the
   comments for getNum() for a description of the arguments to this function. */

int
getInt(const char *arg, int flags, const char *name)
{
    long res;

    res = getNum("getInt", arg, flags, name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);

    return (int) res;
}