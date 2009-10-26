/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <unistd.h> /* isatty */
#include <stdlib.h> /* stdin */
#include <stdio.h>
#include "core_math.h"
#include "version.h"
#include "realmain.h" /* realmain */
#include "scimem.h" /* nofpex */
#include "scilabmode.h"
#include "getcommandlineargs.h"
#include "texmacs.h"
#include "Thread_Wrapper.h"
#include "core_math.h"
#include "setgetlanguage.h"
#include "LaunchScilabSignal.h"

#ifdef __APPLE__
#include "initMacOSXEnv.h"
#endif

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

/*--------------------------------------------------------------------------*/
#define MIN_STACKSIZE 8000000
/*--------------------------------------------------------------------------*/

/*
 * see http://www.gnu.org/software/autoconf/manual/autoconf.html and
 * search for F77_DUMMY_MAIN
 */
#ifdef F77_DUMMY_MAIN
#  ifdef __cplusplus
extern "C"
#  endif
int F77_DUMMY_MAIN() { return 1; }
#endif

int main(int argc, char **argv)
{
  int i;
  int no_startup_flag=0;
  int memory = MIN_STACKSIZE;

  char * initial_script = NULL;
  InitScriptType initial_script_type = SCILAB_SCRIPT;
  /* This bug only occurs under Linux 32 bits
   * See: http://wiki.scilab.org/Scilab_precision
   */
#if defined(linux) && defined(__i386__)
  setFPUToDouble();
#endif

  InitializeLaunchScilabSignal();

#if defined(netbsd) || defined(freebsd)
/* floating point exceptions */
fpsetmask(0);
#endif

#ifdef WITHOUT_GUI
  setScilabMode(SCILAB_NWNI);
#else
  setScilabMode(SCILAB_STD);
#endif

  /* scanning options */
  for ( i=1 ; i < argc ; i++)
  {
      if ( strcmp(argv[i],"-nw") == 0)
      {
      	setScilabMode(SCILAB_NW);
      }
      else if ( strcmp(argv[i],"-nwni") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-display") == 0 || strcmp(argv[i],"-d") == 0)
      {
		  /* @TODO Buffer overflow here */
		  char dpy[128];
		  sprintf(dpy,"DISPLAY=%s",argv[++i]);
		  putenv(dpy);
      }
      else if ( strcmp(argv[i],"-l") == 0)
      {
		  char *argLang=strdup(argv[++i]);

		  /* Export the locale. This is going to be used by setlanguage("") in
			 modules/localization/src/c/InitializeLocalization.c */
		  if (strcmp(argLang,"en")==0) {/* backward compatiblity en => en_US */
			  setenvc("LANG","en_US");
		  }else{
			  if (strcmp(argLang,"fr")==0) { /* backward compatiblity fr => fr_FR */
				  setenvc("LANG","fr_FR");
			  }else{
				  setenvc("LANG",argLang);
			  }
		  }
      }
      else if ( strcmp(argv[i],"-ns") == 0)  { no_startup_flag = 1;}
      else if ( strcmp(argv[i],"-mem") == 0) { i++; memory = Max(atoi(argv[i]),MIN_STACKSIZE );}
      else if ( strcmp(argv[i],"-f") == 0)   { initial_script = argv[++i];}
      else if ( strcmp(argv[i],"-e") == 0)
      {
		  initial_script = argv[++i];
		  initial_script_type = SCILAB_CODE;
      }
      else if ( strcmp(argv[i],"--texmacs") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      	settexmacs();
      }
      else if ( strcmp(argv[i],"-nogui") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-version") == 0) {disp_scilab_version();exit(1);}
    }


  if(!isatty(fileno(stdin)) && getScilabMode() != SCILAB_STD) {

	  /* if not an interactive terminal
	   * then, we are disabling the banner
	   * Since the banner is disabled in the scilab script checking
	   * with the function sciargs is -nb is present, I add this argument
	   * by hand
	   */

	char** pNewArgv = (char**)malloc((argc + 1) * sizeof(char*));

	for(i = 0 ; i < argc ; i++)
	{
		pNewArgv[i] = (char*)malloc((strlen(argv[i]) + 1) * sizeof(char));
		strcpy(pNewArgv[i], argv[i]);
	}
	pNewArgv[i] = (char*)malloc((strlen("-nb") + 1) * sizeof(char));
	strcpy(pNewArgv[i],"-nb");
	setCommandLineArgs(pNewArgv, argc+1);
  }else{
	  setCommandLineArgs(argv, argc);
  }

#ifndef WITH_GUI
  if(getScilabMode() != SCILAB_NWNI)
  {
    fprintf(stderr, "Scilab was compiled without its GUI. Run scilab with the -nwni option.\n");
    exit(1);
  }
#endif

#ifndef __APPLE__
  return realmain(no_startup_flag,initial_script,initial_script_type,memory);
#else
#ifdef WITHOUT_GUI
  /* Do not use this function when building scilab-bin under Mac OS X
   * not that this function is however used by scilab-cli-bin under Mac OS X */
  return realmain(no_startup_flag,initial_script,initial_script_type,memory);
#else
  /* The Mac OS X Java/Swing integration doesn't work the same way as Microsoft Windows or GNU/Linux */
  return initMacOSXEnv(no_startup_flag,initial_script,initial_script_type,memory);
#endif
#endif
}
/*--------------------------------------------------------------------------*/
