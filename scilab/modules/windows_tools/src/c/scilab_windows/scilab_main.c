/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#include "scilab_main.h"
#include "scilabmode.h"
#include "realmain.h"
#include "sciprint.h"
#include "sciquit.h"
#include "LaunchScilabSignal.h"
/*--------------------------------------------------------------------------*/
static void interrupt_setup (void);
static void interrupt (int an_int);
/*--------------------------------------------------------------------------*/
jmp_buf env;
/*--------------------------------------------------------------------------*/
void sci_windows_main ( int *nos, char *path, InitScriptType pathtype, int *lpath, int memory)
{
	InitializeLaunchScilabSignal();
	setbuf (stderr, (char *) NULL);
	if (!setjmp (env))
	{
		/* first time */
		interrupt_setup ();
	}
	/* take commands from stdin */
	realmain(*nos,path,pathtype,memory);
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
void sci_clear_and_exit(int n) /* used with handlers */ 
{
#ifdef _DEBUG
	char Message[256];
	switch (n)
	{
	case SIGINT:
		wsprintf(Message,"SIGINT Signal detected");
		break;
	case SIGILL:
		wsprintf(Message,"SIGILL Signal detected");
		break;
	case SIGFPE:
		wsprintf(Message,"SIGFPE Signal detected");
		break;
	case SIGSEGV:
		wsprintf(Message,"SIGSEGV Signal detected");
		break;
	case SIGTERM:
		wsprintf(Message,"SIGTERM Signal detected");
		break;
	case SIGBREAK:
		wsprintf(Message,"SIGBREAK Signal detected");
		break;
	case SIGABRT:
		wsprintf(Message,"SIGABRT Signal detected");
		break;
	default:
		wsprintf(Message,"Unknow Signal detected");
		break;
	}
	MessageBox(NULL,Message,"ERROR",MB_ICONWARNING);
#else
	/*
	MessageBox(NULL,"Scilab has performed a illegal operation\nand will be shutdown.\n Please save your work ...",
	"Warning",MB_ICONWARNING);
	*/
#endif
	C2F(sciquit)();
}
/*--------------------------------------------------------------------------*/
