/*
  Copyright (C) 2006  EDF - Code Saturne
  Copyright (C) 2001 - DIGITEO - Sylvestre LEDRU. Adapted for Scilab
  Copyright (C) 2016 - Scilab Enterprises - Clement DAVID

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


#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <libintl.h>
#include <pthread.h>

#include <setjmp.h>

#include <sys/types.h>          /* getpid */
#include <sys/time.h>           /* gettimeofday */
#include <unistd.h>             /* gethostname */

#include "csignal.h"
#include "localization.h"
#include "backtrace.h"
#include "signal_mgmt.h"
#include "Scierror.h"
#include "suspendProcess.h"
#include "configvariable_interface.h"
#include "backtrace_print.h"
#include "cliDisplayManagement.h"
#include "initConsoleMode.h"
#include "exit_status.hxx"

jmp_buf ScilabJmpEnv;

/*----------------------------------------------------------------------------
 * Handle a fatal signal (such as SIGFPE or SIGSEGV)
 *----------------------------------------------------------------------------*/
#define HOSTFORMAT "[%s:%05d] "
static void sig_fatal(int signum, siginfo_t * info, void *p)
{
    char *si_code_str = "";

    int ret, i;

    char print_buffer[2048];

    int size = sizeof(print_buffer);

    char *tmp = print_buffer;

    char stacktrace_hostname[64];

    char* bt;

    gethostname(stacktrace_hostname, sizeof(stacktrace_hostname));
    stacktrace_hostname[sizeof(stacktrace_hostname) - 1] = '\0';
    /* to keep these somewhat readable, only print the machine name */
    for (i = 0;
            i < (int)sizeof(stacktrace_hostname) && stacktrace_hostname[i] != '\0';
            ++i)
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
    {
        char* str;
        str = strsignal(signum);
        ret = snprintf(tmp, size, HOSTFORMAT "Signal: %s (%d)\n", stacktrace_hostname, getpid(), str, signum);
    }
#else
    ret = snprintf(tmp, size, HOSTFORMAT "Signal: %d\n", stacktrace_hostname, getpid(), signum);
#endif
    tmp += ret;
    size -= ret;

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
            tmp += ret;
            size -= ret;
        }

        ret = snprintf(tmp, size, HOSTFORMAT "Signal code: %s (%d)\n", stacktrace_hostname, getpid(), si_code_str, info->si_code);
        tmp += ret;
        size -= ret;

        switch (signum)
        {
            case SIGILL:
            case SIGFPE:
            case SIGSEGV:
            case SIGBUS:
            {
                ret = snprintf(tmp, size, HOSTFORMAT "Failing at address: %p\n", stacktrace_hostname, getpid(), info->si_addr);
                break;
            }
            case SIGCHLD:
            {
                snprintf(tmp, size, HOSTFORMAT "Sending PID: %d, Sending UID: %d, Status: %d\n",
                         stacktrace_hostname, getpid(), info->si_pid, info->si_uid, info->si_status);
                break;
            }
#ifdef SIGPOLL
            case SIGPOLL:
            {
#ifdef HAVE_SIGINFO_T_SI_FD
                snprintf(tmp, size, HOSTFORMAT "Band event: %ld, File Descriptor : %d\n",
                         stacktrace_hostname, getpid(), info->si_band, info->si_fd);
#elif HAVE_SIGINFO_T_SI_BAND
                snprintf(tmp, size, HOSTFORMAT "Band event: %ld\n", stacktrace_hostname, getpid(), info->si_band);
#endif
                break;
            }
#endif
        }
    }
    else
    {
        snprintf(tmp, size, HOSTFORMAT "siginfo is NULL, additional information unavailable\n", stacktrace_hostname, getpid());
    }

    // 4 is to ignore the first 4 functions
    bt = backtrace_print(4, 1);
    Scierror(42,
             _("A fatal error has been detected by Scilab.\nPlease check your user-defined functions (or external module ones) should they appear in the stack trace.\nOtherwise you can report a bug on %s with:\n * a sample code which reproduces the issue\n * the result of [a, b] = getdebuginfo()\n * the following information:\n%s %s\n"),
             PACKAGE_BUGREPORT, print_buffer, bt);

    free(bt);

    if (getScilabMode() == SCILAB_NWNI || getScilabMode() == SCILAB_NW)
    {
        /* Reset termcaps and Characters display. */
        setAttr(ATTR_RESET);
        setCharDisplay(DISP_RESET);
    }

    longjmp(ScilabJmpEnv, HUGE_ERROR);
}

void base_error_init(void)
{
    struct sigaction act;
    int j;
    struct sigaction ToSuspend;
    struct sigaction ToContinue;
    int signals[] =
    {
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

    /* Initialise Suspend Signal (CTRL-Z) */
    ToSuspend.sa_handler = suspendProcess;
    ToSuspend.sa_flags = 0;
    sigemptyset(&ToSuspend.sa_mask);
    sigaction(SIGTSTP, &ToSuspend, NULL);
    /* Initialise Continue Signal (fg) */
    ToContinue.sa_handler = continueProcess;
    ToContinue.sa_flags = 0;
    sigemptyset(&ToContinue.sa_mask);
    sigaction(SIGCONT, &ToContinue, NULL);
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
    sigemptyset(&act.sa_mask);

    for (j = 0; signals[j] != -1; ++j)
    {
        if (0 != sigaction(signals[j], &act, NULL))
        {
            fprintf(stderr, "Could not set handler for signal %d\n", signals[j]);
        }
    }

#ifdef HAVE_STRSIGNAL
    // initialize the glibc internal string representation.
    strsignal(SIGABRT);
#endif
}

static void* watchdog_thread(void* arg)
{
    long timeoutDelay = (long) arg;

    pthread_mutex_t watchdog_mutex;
    pthread_cond_t dummy_condition;
    struct timeval tv;
    struct timespec abstime;

    if (pthread_mutex_init(&watchdog_mutex, NULL) != 0)
    {
        return NULL;
    }

    if (pthread_cond_init(&dummy_condition, NULL) != 0)
    {
        pthread_mutex_destroy(&watchdog_mutex);
        return NULL;
    }

    if (gettimeofday(&tv, NULL) != 0)
    {
        pthread_cond_destroy(&dummy_condition);
        pthread_mutex_destroy(&watchdog_mutex);
        return NULL;
    }

    memset(&abstime, 0, sizeof(struct timespec));
    abstime.tv_sec = tv.tv_sec + timeoutDelay;

    while (1)
    {
        if (pthread_cond_timedwait(&dummy_condition, &watchdog_mutex, &abstime) == ETIMEDOUT)
        {
            /*
             * Send a SIGABRT to ensure process termination, if used with the signal
             * trap a backtrace might be displayed.
             */
            kill(getpid(), SIGABRT);
        }
    }
    return NULL;
}

void timeout_process_after(int timeoutDelay)
{
    pthread_t watchdog;

    // Spawn a watchdog thread as POSIX timer API is not available on MacOS X
    if (pthread_create(&watchdog, NULL, watchdog_thread, (void*) (long) timeoutDelay) != 0)
    {
        return;
    }
}

/*--------------------------------------------------------------------------*/
