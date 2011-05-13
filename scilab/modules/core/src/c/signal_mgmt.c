/*
  Copyright (C) 2006  EDF - Code Saturne
  Copyright (C) 2001 - DIGITEO - Sylvestre LEDRU. Adapted for Scilab

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include <libintl.h>

#if defined(__linux__)
#define __USE_FORTIFY_LEVEL 0 /* Avoid dependency on GLIBC_2.11 (__longjmp_chk) */
#endif
#include <setjmp.h> /* this declaration should remain close the __USE_FORTIFY_LEVEL define */

#include <sys/types.h> /* getpid */
#include <unistd.h> /* gethostname */

#include "csignal.h"
#include "localization.h"
#include "backtrace.h"
#include "signal_mgmt.h"
#include "machine.h"
#include "Scierror.h"
extern jmp_buf jmp_env;
/*----------------------------------------------------------------------------
 * Print a stack trace
 *----------------------------------------------------------------------------*/

static char *backtrace_print(int niv_debut)
{
    size_t ind;
    sci_backtrace_t *tr = NULL;
    char print_buffer[4096]; /* TODO: make it dynamic */

    int size = sizeof(print_buffer);
    char *tmp = print_buffer;
    int ret;

    tr = sci_backtrace_create();

    if (tr != NULL)
    {

        char s_func_buf[67];

        const char *s_file;
        const char *s_func;
        const char *s_addr;

        const char s_unknown[] = "?";
        const char s_vide[] = "";
        const char *s_prefix = s_vide;

        size_t nbr = sci_backtrace_size(tr);

        if (nbr > 0)
        {
            ret = snprintf(tmp, size, _("\nCall stack:\n"));
            size -= ret;
            tmp += ret;

        }

        for (ind = niv_debut; ind < nbr; ind++)
        {

            s_file = sci_backtrace_file(tr, ind);
            s_func = sci_backtrace_function(tr, ind);
            s_addr = sci_backtrace_address(tr, ind);

            if (s_file == NULL)
            {
                s_file = s_unknown;
            }
            if (s_func == NULL)
            {
                strcpy(s_func_buf, "?");
            }
            else
            {
                s_func_buf[0] = '<';
                strncpy(s_func_buf + 1, s_func, 64);
                strcat(s_func_buf, ">");
            }

            if (s_addr == NULL)
            {
                s_addr = s_unknown;
            }

            ret = snprintf(tmp, size, "%s%4lu: %-16s %-32s (%s)\n", s_prefix, ind - niv_debut + 1, s_addr, s_func_buf, s_file);
            size -= ret;
            tmp += ret;
        }

        sci_backtrace_destroy(tr);

        if (nbr > 0)
        {
            ret = snprintf(tmp, size, _("End of stack\n\n"));
            size -= ret;
            tmp += ret;
        }
    }

    return strdup(print_buffer);

}

/*----------------------------------------------------------------------------
 * Handle a fatal signal (such as SIGFPE or SIGSEGV)
 *----------------------------------------------------------------------------*/
#define HOSTFORMAT "[%s:%05d] "
static void sig_fatal(int signum, siginfo_t * info, void *p)
{
    char *si_code_str = "";
    int ret, i;
    char print_buffer[1024];
    int size = sizeof(print_buffer);

    char *tmp = print_buffer;
    char stacktrace_hostname[64];

    gethostname(stacktrace_hostname, sizeof(stacktrace_hostname));
    stacktrace_hostname[sizeof(stacktrace_hostname) - 1] = '\0';
    /* to keep these somewhat readable, only print the machine name */
    for (i = 0; i < (int)sizeof(stacktrace_hostname); ++i)
    {
        if (stacktrace_hostname[i] == '.')
        {
            stacktrace_hostname[i] = '\0';
            break;
        }
    }

    fflush(stdout);
    memset(print_buffer, 0, sizeof(print_buffer));


    /* This list comes from OpenMPI sources */
#ifdef HAVE_STRSIGNAL
    ret = snprintf(tmp, size, HOSTFORMAT "Signal: %s (%d)\n", stacktrace_hostname, getpid(), strsignal(signum), signum);
#else
    ret = snprintf(tmp, size, HOSTFORMAT "Signal: %d\n", stacktrace_hostname, getpid(), signum);
#endif

    size -= ret;
    tmp += ret;

    if (NULL != info)
    {
        switch (signum)
        {
        case SIGILL:
            switch (info->si_code)
            {
#ifdef ILL_ILLOPC
            case ILL_ILLOPC:
                si_code_str = "Illegal opcode";
                break;
#endif
#ifdef ILL_ILLOPN
            case ILL_ILLOPN:
                si_code_str = "Illegal operand";
                break;
#endif
#ifdef ILL_ILLADR
            case ILL_ILLADR:
                si_code_str = "Illegal addressing mode";
                break;
#endif
#ifdef ILL_ILLTRP
            case ILL_ILLTRP:
                si_code_str = "Illegal trap";
                break;
#endif
#ifdef ILL_PRVOPC
            case ILL_PRVOPC:
                si_code_str = "Privileged opcode";
                break;
#endif
#ifdef ILL_PRVREG
            case ILL_PRVREG:
                si_code_str = "Privileged register";
                break;
#endif
#ifdef ILL_COPROC
            case ILL_COPROC:
                si_code_str = "Coprocessor error";
                break;
#endif
#ifdef ILL_BADSTK
            case ILL_BADSTK:
                si_code_str = "Internal stack error";
                break;
#endif
            }
            break;
        case SIGFPE:
            switch (info->si_code)
            {
#ifdef FPE_INTDIV
            case FPE_INTDIV:
                si_code_str = "Integer divide-by-zero";
                break;
#endif
#ifdef FPE_INTOVF
            case FPE_INTOVF:
                si_code_str = "Integer overflow";
                break;
#endif
            case FPE_FLTDIV:
                si_code_str = "Floating point divide-by-zero";
                break;
            case FPE_FLTOVF:
                si_code_str = "Floating point overflow";
                break;
            case FPE_FLTUND:
                si_code_str = "Floating point underflow";
                break;
#ifdef FPE_FLTRES
            case FPE_FLTRES:
                si_code_str = "Floating point inexact result";
                break;
#endif
#ifdef FBE_FLTINV
            case FPE_FLTINV:
                si_code_str = "Invalid floating point operation";
                break;
#endif
#ifdef FPE_FLTSUB
            case FPE_FLTSUB:
                si_code_str = "Subscript out of range";
                break;
#endif
            }
            break;
        case SIGSEGV:
            switch (info->si_code)
            {
#ifdef SEGV_MAPERR
            case SEGV_MAPERR:
                si_code_str = "Address not mapped";
                break;
#endif
#ifdef SEGV_ACCERR
            case SEGV_ACCERR:
                si_code_str = "Invalid permissions";
                break;
#endif
            }
            break;
        case SIGBUS:
            switch (info->si_code)
            {
#ifdef BUS_ADRALN
            case BUS_ADRALN:
                si_code_str = "Invalid address alignment";
                break;
#endif
#ifdef BUSADRERR
            case BUS_ADRERR:
                si_code_str = "Non-existent physical address";
                break;
#endif
#ifdef BUS_OBJERR
            case BUS_OBJERR:
                si_code_str = "Objet-specific hardware error";
                break;
#endif
            }
            break;
        case SIGTRAP:
            switch (info->si_code)
            {
#ifdef TRAP_BRKPT
            case TRAP_BRKPT:
                si_code_str = "Process breakpoint";
                break;
#endif
#ifdef TRAP_TRACE
            case TRAP_TRACE:
                si_code_str = "Process trace trap";
                break;
#endif
            }
            break;
        case SIGCHLD:
            switch (info->si_code)
            {
#ifdef CLD_EXITED
            case CLD_EXITED:
                si_code_str = "Child has exited";
                break;
#endif
#ifdef CLD_KILLED
            case CLD_KILLED:
                si_code_str = "Child has terminated abnormally and did not create a core file";
                break;
#endif
#ifdef CLD_DUMPED
            case CLD_DUMPED:
                si_code_str = "Child has terminated abnormally and created a core file";
                break;
#endif
#ifdef CLD_WTRAPPED
            case CLD_TRAPPED:
                si_code_str = "Traced child has trapped";
                break;
#endif
#ifdef CLD_STOPPED
            case CLD_STOPPED:
                si_code_str = "Child has stopped";
                break;
#endif
#ifdef CLD_CONTINUED
            case CLD_CONTINUED:
                si_code_str = "Stopped child has continued";
                break;
#endif
            }
            break;
#ifdef SIGPOLL
        case SIGPOLL:
            switch (info->si_code)
            {
#ifdef POLL_IN
            case POLL_IN:
                si_code_str = "Data input available";
                break;
#endif
#ifdef POLL_OUT
            case POLL_OUT:
                si_code_str = "Output buffers available";
                break;
#endif
#ifdef POLL_MSG
            case POLL_MSG:
                si_code_str = "Input message available";
                break;
#endif
#ifdef POLL_ERR
            case POLL_ERR:
                si_code_str = "I/O error";
                break;
#endif
#ifdef POLL_PRI
            case POLL_PRI:
                si_code_str = "High priority input available";
                break;
#endif
#ifdef POLL_HUP
            case POLL_HUP:
                si_code_str = "Device disconnected";
                break;
#endif
            }
            break;
#endif /* SIGPOLL */
        default:
            switch (info->si_code)
            {
#ifdef SI_ASYNCNL
            case SI_ASYNCNL:
                si_code_str = "SI_ASYNCNL";
                break;
#endif
#ifdef SI_SIGIO
            case SI_SIGIO:
                si_code_str = "Queued SIGIO";
                break;
#endif
#ifdef SI_ASYNCIO
            case SI_ASYNCIO:
                si_code_str = "Asynchronous I/O request completed";
                break;
#endif
#ifdef SI_MESGQ
            case SI_MESGQ:
                si_code_str = "Message queue state changed";
                break;
#endif
            case SI_TIMER:
                si_code_str = "Timer expiration";
                break;
            case SI_QUEUE:
                si_code_str = "Sigqueue() signal";
                break;
            case SI_USER:
                si_code_str = "User function (kill, sigsend, abort, etc.)";
                break;
#ifdef SI_KERNEL
            case SI_KERNEL:
                si_code_str = "Kernel signal";
                break;
#endif
#ifdef SI_UNDEFINED
            case SI_UNDEFINED:
                si_code_str = "Undefined code";
                break;
#endif
            }
        }

        /* print signal errno information */
        if (0 != info->si_errno)
        {
            ret = snprintf(tmp, size, HOSTFORMAT "Associated errno: %s (%d)\n",
                           stacktrace_hostname, getpid(), strerror(info->si_errno), info->si_errno);
            size -= ret;
            tmp += ret;
        }

        ret = snprintf(tmp, size, HOSTFORMAT "Signal code: %s (%d)\n", stacktrace_hostname, getpid(), si_code_str, info->si_code);
        size -= ret;
        tmp += ret;

        switch (signum)
        {
        case SIGILL:
        case SIGFPE:
        case SIGSEGV:
        case SIGBUS:
            {
                ret = snprintf(tmp, size, HOSTFORMAT "Failing at address: %p\n", stacktrace_hostname, getpid(), info->si_addr);
                size -= ret;
                tmp += ret;
                break;
            }
        case SIGCHLD:
            {
                ret = snprintf(tmp, size, HOSTFORMAT "Sending PID: %d, Sending UID: %d, Status: %d\n",
                               stacktrace_hostname, getpid(), info->si_pid, info->si_uid, info->si_status);
                size -= ret;
                tmp += ret;
                break;
            }
#ifdef SIGPOLL
        case SIGPOLL:
            {
#ifdef HAVE_SIGINFO_T_SI_FD
                ret = snprintf(tmp, size, HOSTFORMAT "Band event: %ld, File Descriptor : %d\n",
                               stacktrace_hostname, getpid(), info->si_band, info->si_fd);
#elif HAVE_SIGINFO_T_SI_BAND
                ret = snprintf(tmp, size, HOSTFORMAT "Band event: %ld\n", stacktrace_hostname, getpid(), info->si_band);
#else
                ret = 0;
#endif
                size -= ret;
                tmp += ret;
                break;
            }
#endif
        }
    }
    else
    {
        ret = snprintf(tmp, size, HOSTFORMAT "siginfo is NULL, additional information unavailable\n", stacktrace_hostname, getpid());
        size -= ret;
        tmp += ret;
    }
    Scierror(42, _("Oups. A fatal error has been detected by Scilab.\nYour instance will probably crash soon.\nPlease report a bug on %s with the following\ninformation:\n%s %s\n"), PACKAGE_BUGREPORT, print_buffer,
             backtrace_print(0));

    longjmp(&jmp_env, 1);

}

void base_error_init(void)
{
    struct sigaction act;
    int sig, j;

    /* Signal handlers */
    csignal();

    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sig_fatal;
    act.sa_flags = SA_SIGINFO;
#ifdef SA_ONESHOT
    act.sa_flags |= SA_ONESHOT;
#else
    act.sa_flags |= SA_RESETHAND;
#endif

    int signals[] = {
#ifdef SIGABRT
        SIGABRT,
#endif
#ifdef SIGBUS
        SIGBUS,
#endif
#ifdef SIGFPE
        SIGFPE,
#endif
#ifdef SIGFPE2
        SIGFPE,
#endif
#ifdef SIGILL
        SIGILL,
#endif
#ifdef SIGSEGV
        SIGSEGV,
#endif
#ifdef SIGPOLL
        SIGPOLL,
#endif
        -1
    };
    for (j = 0; signals[j] != -1; ++j)
    {
        if (0 != sigaction(signals[j], &act, NULL))
        {
            fprintf(stderr,"Could not set handler for signal %d\n",signals[j]);
        }
    }
}


/*--------------------------------------------------------------------------*/
