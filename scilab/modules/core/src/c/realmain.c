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

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

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
#define BSIZE 128
/*--------------------------------------------------------------------------*/
/* See the function for the explanation */
#ifdef linux
#ifndef IS_64_BITS_CPU
#include <fpu_control.h>

static void checkPresenceOfBug3443(void);
static void set_fpu_64(void);
#endif
#endif
/*--------------------------------------------------------------------------*/
void realmain(int no_startup_flag_l, char *initial_script, InitScriptType initial_script_type, int memory)
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

  /* This bug only occurs under Linux 32 bits */
#if defined(linux) && defined(__i386__)
  setFPUToDouble();
#endif

#if !defined(_DEBUG) && defined(_MSC_VER)
  _try
    {
      C2F(scirun)(startup,(long int)strlen(startup));
    }
  _except (EXCEPTION_EXECUTE_HANDLER)
    {
    Rerun:
      {
	ExceptionMessage(GetExceptionCode(),NULL);
      }
      _try
	{
	  C2F(scirun)("",(long int)strlen(""));
	}
      _except (EXCEPTION_EXECUTE_HANDLER)
	{
	  goto Rerun;
	}

    }
#else
  C2F(scirun)(startup,(long int)strlen(startup));
#endif

  FREE(startup);
  /* cleaning */
  C2F(sciquit)();
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
#ifdef linux
#ifndef IS_64_BITS_CPU
/*
 * This function is an standalone function which aims to see
 * if the bug 3443 ( http://bugzilla.scilab.org/show_bug.cgi?id=3443 )
 * is present.
 * As far as we know, this bug is only present under Linux 32 bits
 * Basically, the Java/JNI function JNI_CreateJavaVM which is changing the
 * Precision Control (PC) of the FPU.
  * Then, the goal of this function is:
 * - Check the bug exist
 * - If it does exist, change the PC of the FPU (Assembler... Yep, I know)
 * - Check if the bug is actually fixed.
 * - If not fixed, you are screwed and we would like to the know your
 *   configuration
 *
 * Note that this function is not used because the first case is always true
 * (which is not the case in a stand alone code)
 */
static void checkPresenceOfBug3443(void)
{
	/* Our test case value */
	double val1=8.8353017163510351e-10;
	double sqrtWithPow=0.5;

	if (sqrt(val1) != pow(val1,sqrtWithPow)) {
		/* We are in the case of the bug */
		set_fpu_64();
		if (sqrt(val1) != pow(val1,sqrtWithPow)) {
			fprintf(stderr,"\nThis case should NOT occur. You are faced to the bug #3443. Please report to Scilab's bugtracker ( http://bugzilla.scilab.org/show_bug.cgi?id=3443 ) with your CPU (cat /proc/cpuinfo), your operating system, your kernel (uname -a) and the result of the Scilab command 'ver'.\n");
		}
	}

}


/* Set the FPU to 64 bits... It is the expected behaviour of the lib math
 * and thefore Scilab behaviour */
static void set_fpu_64(void)
{
	fpu_control_t _cw;
    _FPU_GETCW(_cw);
	_cw = (_cw & ~_FPU_DOUBLE) | _FPU_EXTENDED;
    _FPU_SETCW(_cw);
}

#endif
#endif
