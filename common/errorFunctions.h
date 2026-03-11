#define _GNU_SOURCE
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

#include <stdarg.h>

/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
\*************************************************************************/

/* error_functions.h

   Header file for standard error handling routines used by
   various programs.
*/

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean;

/* Function prototypes */

void errMsg(const char *format, ...);

void errExit(const char *format, ...);

void err_exit(const char *format, ...);

void errExitEN(int errnum, const char *format, ...);

void fatal(const char *format, ...);

void usageErr(const char *format, ...);

void cmdLineErr(const char *format, ...);

#endif