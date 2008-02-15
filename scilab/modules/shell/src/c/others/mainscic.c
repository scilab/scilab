/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CONRET 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "math.h"
#include "version.h"
#include "realmain.h" /* realmain */
#include "scimem.h" /* nofpex */
#include "scilabmode.h"
#include "getcommandlineargs.h"
#include "texmacs.h"
#include "x_main.h"
/*--------------------------------------------------------------------------*/
#define MIN_STACKSIZE 180000
/*--------------------------------------------------------------------------*/
char *ProgramName;
/*--------------------------------------------------------------------------*/
int  sci_show_banner=1;
/*--------------------------------------------------------------------------*/
void mainscic(int argc, char **argv)
{
  int i;
  int  no_startup_flag=0;
  int  memory = MIN_STACKSIZE;

  char * initial_script = NULL;
  InitScriptType initial_script_type = SCILAB_SCRIPT;

  char  *display = NULL;

  #if (defined __GNUC__  )
		putenv ("COMPILER=gcc");
	#else
		putenv ("COMPILER=cc or another");
	#endif

#if defined(netbsd) || defined(freebsd)
/* floating point exceptions */
fpsetmask(0);
#endif

  ProgramName = argv[0];
#ifdef WITHOUT_GUI
  setScilabMode(SCILAB_NWNI);
#else
  setScilabMode(SCILAB_STD);
#endif

  setCommandLineArgs(argv, argc);


  /* scanning options */
  for ( i=0 ; i < argc ; i++)
  {
      if ( strcmp(argv[i],"-nw") == 0)
      {
      	setScilabMode(SCILAB_NW);
      }
      else if ( strcmp(argv[i],"-nwni") == 0)
      {
      	setScilabMode(SCILAB_NWNI);
      }
      else if ( strcmp(argv[i],"-display") == 0)
      {
		  char dpy[128];
		  sprintf(dpy,"DISPLAY=%s",display);
		  putenv(dpy);
      }
      else if ( strcmp(argv[i],"-nb") == 0)  { sci_show_banner = 0; }
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

  realmain(no_startup_flag,initial_script,initial_script_type,memory);
}
/*--------------------------------------------------------------------------*/
