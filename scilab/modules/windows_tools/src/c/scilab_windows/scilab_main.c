/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#include "scilab_main.h"
#include "sci_mode.h"
#include "realmain.h"
#include "sciprint.h"
#include "LaunchScilabSignal.h"
/*--------------------------------------------------------------------------*/
static void interrupt_setup (void);
static void interrupt (int an_int);
/*--------------------------------------------------------------------------*/
jmp_buf env;
/*--------------------------------------------------------------------------*/
int sci_windows_main ( int *nos, char *path, InitScriptType pathtype, int *lpath, int memory)
{
    InitializeLaunchScilabSignal();
    setbuf (stderr, (char *) NULL);
    if (!setjmp (env))
    {
        /* first time */
        interrupt_setup ();
    }
    /* take commands from stdin */
    return 0;//realmain(*nos,path,pathtype,memory);
}
/*--------------------------------------------------------------------------*/
/* Set up to catch interrupts */
static void interrupt_setup (void)
{
    (void) signal (SIGINT, interrupt);
}
/*--------------------------------------------------------------------------*/
void interrupt (int an_int)
{
    (void) signal (SIGINT, interrupt);
    (void) signal (SIGFPE, SIG_DFL);	/* turn off FPE trapping */
    (void) fflush (stdout);
    sciprint ("\n");
    longjmp (env, TRUE);		/* return to prompt  */
}
/*--------------------------------------------------------------------------*/
