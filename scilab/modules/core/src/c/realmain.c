/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "realmain.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "inffic.h" /* get_sci_data_strings */
#include "scirun.h"
#include "realmain.h" /* enum InitScriptType */
#include "sciquit.h"
#include "tmpdir.h"
#include "inisci-c.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
extern void sci_clear_and_exit(int n);
extern void sci_usr1_signal(int n);
#ifdef _MSC_VER
#include "ExceptionMessage.h"
#define snprintf _snprintf
#endif
/*--------------------------------------------------------------------------*/
static int no_startup_flag=0;
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int callScirun(char *startupCommand);
#endif
/*--------------------------------------------------------------------------*/
int realmain(int no_startup_flag_l, char *initial_script, InitScriptType initial_script_type, int memory)
{
  static int initialization=-1;
  int ierr=0;
  char *startup=(char*)MALLOC(sizeof(char)*(PATH_MAX+1));
  Set_no_startup_flag(no_startup_flag_l);

  /* create temp directory */
  C2F(settmpdir)();

  /* signals */
#ifdef _MSC_VER
  /* Don't want catch signal under Linux ... */
#ifdef ENABLESIG
#ifndef DEBUG
  signal(SIGINT,sci_clear_and_exit);
#ifdef SIGBUS
  signal(SIGBUS,sci_clear_and_exit);
#endif
  signal(SIGSEGV,sci_clear_and_exit);
#if SIGQUIT
  signal(SIGQUIT,sci_clear_and_exit);
#endif
#ifdef SIGHUP
  signal(SIGHUP,sci_clear_and_exit);
#endif
#ifdef SIGUSR1
  signal(SIGUSR1,sci_usr1_signal);
#endif
#endif

#ifdef _MSC_VER
  signal(SIGILL,sci_clear_and_exit);
  signal(SIGFPE,sci_clear_and_exit);
  signal(SIGTERM,sci_clear_and_exit);
  signal(SIGBREAK,sci_clear_and_exit);
  signal(SIGABRT,sci_clear_and_exit);
#endif
#endif
#endif

  /*  prepare startup script  */

  if ( no_startup_flag_l == 0)
    {
      /* execute a startup */
      if ( initial_script != NULL )
	{
	  switch ( initial_script_type )
	    {
	    case SCILAB_SCRIPT :
	      snprintf(startup,PATH_MAX,"%s;exec('%s',-1)",get_sci_data_strings(STARTUP_ID),initial_script);
	      break;
	    case SCILAB_CODE :
	      snprintf(startup,PATH_MAX,"%s;%s;",get_sci_data_strings(STARTUP_ID),initial_script);
	      break;
	    }
	}
      else
	{
	  snprintf(startup,PATH_MAX,"%s;",get_sci_data_strings(STARTUP_ID));
	}
    }
  else
    {
      /* No startup but maybe an initial script  */
      if ( initial_script != NULL )
	{
	  switch ( initial_script_type )
	    {
	    case SCILAB_SCRIPT :
	      snprintf(startup,PATH_MAX,"exec('%s',-1)",initial_script);
	      break;
	    case SCILAB_CODE :
	      snprintf(startup,PATH_MAX,"%s;",initial_script);
	      break;
	    }
	}
      else sprintf(startup," ");
    }

  startup[PATH_MAX] = '\0'; /* force string to be null-terminated on overflow */

  /* initialize scilab interp  */
  C2F(inisci)(&initialization, &memory, &ierr);
  if (ierr > 0) C2F(sciquit)() ;
  /* execute the initial script and enter scilab */

#if !defined(_DEBUG) && defined(_MSC_VER)
  /* if scilab crashs by a exception , we try to quit properly */
  /* Windows release mode */
  callScirun(startup);
#else
  C2F(scirun)(startup,(long int)strlen(startup));
#endif

  FREE(startup);
  /* cleaning */
  C2F(sciquit)();
  /* Actually manage the return code */
  return 0;
}
/*--------------------------------------------------------------------------*/
void Set_no_startup_flag(int start)
{
  no_startup_flag=start;
}
/*--------------------------------------------------------------------------*/
int Get_no_startup_flag(void)
{
  return no_startup_flag;
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static int callScirun(char *startupCommand)
{
	_try
	{
		C2F(scirun)(startupCommand,(long int)strlen(startupCommand));
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{
		DWORD dwExceptionCode = GetExceptionCode();
Rerun:
		ExceptionMessage(GetExceptionCode(),NULL);
		if (dwExceptionCode != EXCEPTION_ACCESS_VIOLATION)
		{
			_try
			{
				C2F(scirun)("",(long int)strlen(""));
			}

			_except (EXCEPTION_EXECUTE_HANDLER)
			{
				goto Rerun;
			}
		}
	}
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
