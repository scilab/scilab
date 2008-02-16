/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "javasci_globals.h"
#include "setgetSCIpath.h"
#include "getcommandlineargs.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/SetScilabEnvironmentVariables.h"
#endif

static int init = 0;
/*****************************************************************************/
void EnableInterf()
{
	init = 1;
}
/*****************************************************************************/
void DisableInterf()
{
	init = 0;
}
/*****************************************************************************/
int GetInterfState()
{
	return init;
}
/**
 * Initialisation of Scilab 
 */
void Initialize(void) 
{
  static char env[1024];
  static char initstr[]="exec(\"SCI/etc/scilab.start\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;

  #ifdef _MSC_VER
	#define nb_args 3
	static char *argv[nb_args];
    static char JavaSciInterf[]="javasci";
    static char nw[]="-nw";
    static char nb[]="-nb";
  #endif

  
  char *p1 = (char*)getenv ("SCI");
  
  
  #ifdef _MSC_VER
  /* Delete the windows mode and the banner */
  {
    argv[0] = JavaSciInterf;
	argv[1] = nw;
	argv[2] = nb;
	setCommandLineArgs(argv, nb_args);
  }
  
  #endif
  
  #ifdef _MSC_VER 
    if ( p1== NULL )
    {
		/* Detection Scilab path */
		char modname[PATH_MAX+1];
		if (!GetModuleFileName (GetModuleHandle("javasci.dll"), modname, PATH_MAX))
		{
			MessageBox(NULL,"javasci.dll not found","Warning",MB_ICONWARNING);
		}
		else
		{
			char *p;
			if ((p = strrchr (modname, '\\')) == NULL) exit(1); /* remove \javasci.dll from modname */
			else
			{
				*p='\0';
				if ((p = strrchr (modname, '\\')) == NULL) exit(1); /* remove \bin from modname */
				else
				{
					*p='\0';
					SetScilabEnvironmentVariables(modname);
				}
			}
		}
    }
    else 
	{
		char *pathSCI=(char*)MALLOC((strlen(p1)+1)*sizeof(char));
		strcpy(pathSCI,p1);
		SetScilabEnvironmentVariables(pathSCI);
		if (pathSCI) {FREE(pathSCI);pathSCI=NULL;}
	}
  #else
   if (p1==NULL)
   {
   	fprintf(stderr,"Please define SCI environment variable\n");
   	sprintf (env, "%s=%s", "SCI",SCI);
	setSCIpath(SCI);
	putenv (env);
   }
  #endif
  /* set TMPDIR */
  C2F(settmpdir)();

  /* For the initilization of the Scilab primivite : fromjava() */
  SetFromJavaToON();

  /* Scilab Initialization */
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }

  /* Load of Scilab.start */
  C2F(scirun)(initstr,(int)strlen(initstr));
 
}
/*****************************************************************************/
int send_scilab_job(char *job) 
{
  return (int) SendScilabJob(job); 
}
/*****************************************************************************/
