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
#include <stdlib.h>
#include <string.h>
#include "machine.h"
#ifndef _MSC_VER
#ifdef WITH_TK
#include <tcl.h>
#endif
#include "isdir.h"
#endif
#include "javasci_globals.h"
#include "inisci-c.h"
#include "setgetSCIpath.h"
#include "tmpdir.h"
#include "PATH_MAX.h"
#include "getcommandlineargs.h"
#ifdef _MSC_VER
#include "../../../windows_tools/src/c/scilab_windows/SetScilabEnvironmentVariables.h"
#include "strdup_windows.h"
#endif
#include "LaunchScilabSignal.h"
/*--------------------------------------------------------------------------*/
static int init = 0;
/*--------------------------------------------------------------------------*/
void EnableInterf(void)
{
	init = 1;
}
/*--------------------------------------------------------------------------*/
void DisableInterf(void)
{
	init = 0;
}
/*--------------------------------------------------------------------------*/
int GetInterfState(void)
{
	return init;
}
/*--------------------------------------------------------------------------*/
void Initialize(void)
{
	static char env[1024];
	static char initstr[]="exec(\"SCI/etc/scilab.start\",-1);quit;";
	static int iflag = -1, stacksize = 1000000, ierr = 0;

#ifdef _MSC_VER
#define nb_args 3
	static char *argv[nb_args];
	static char JavaSciInterf[]="javasci";
	static char nw[]="-nw";
	static char nb[]="-nb";
#endif


	char *sciPath = (char*)getenv("SCI");

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
	if ( sciPath == NULL )
	{
		/* Detection Scilab path */
		char modname[PATH_MAX+1];
		if (!GetModuleFileName (GetModuleHandle("javasci.dll"), modname, PATH_MAX))
		{
			MessageBox(NULL,"javasci.dll not found","Warning",MB_ICONWARNING);
		}
		else
		{
			char *p = NULL;
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
		char *pathSCI = strdup(sciPath);
		SetScilabEnvironmentVariables(pathSCI);
		if (pathSCI) { FREE(pathSCI); pathSCI  =NULL; }
	}
#else
	if (sciPath==NULL)
	{
		fprintf(stderr,"Please define SCI environment variable\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		sprintf (env, "%s=%s", "SCI", sciPath);
		setSCIpath(SCI);
		putenv (env);
	}

	/*
	* This stuff is a workaround when the user is using javasci in the Scilab
	* official binary
	* The problem in this case is that the TCL_LIBRARY & TK_LIBRARY are not
	* set correctly and TCL has trouble to find himself.
	*
	* As we are sure to use the binary, we know where to find
	* See: http://bugzilla.scilab.org/show_bug.cgi?id=3605
	*/
#ifndef _MSC_VER
#ifdef WITH_TK
#define BASEPATHTOTHIRDPARTY "/../../thirdparty/"
#define DIRECTORYOFTCL "tcl"
#define DIRECTORYOFTK "tk"
	int commonPart=strlen(sciPath)+strlen(BASEPATHTOTHIRDPARTY)+strlen(".")+strlen(TCL_VERSION)+1;
	char *pathToTcl=(char*)MALLOC((strlen(DIRECTORYOFTCL)+commonPart)*sizeof(char));
	sprintf(pathToTcl, "%s%s%s%s", sciPath, BASEPATHTOTHIRDPARTY, DIRECTORYOFTCL, TCL_VERSION);

	/* Test if we find the thirdparty directory. If it is the case, it is most
	* probable that we are working with the binary version of Scilab
	*/
	if (isdir(pathToTcl)){
#define EXPORTTCL "TCL_LIBRARY="
#define EXPORTTK "TK_LIBRARY="
		char *pathToTk=(char*)MALLOC((strlen(DIRECTORYOFTK)+commonPart)*sizeof(char));
		sprintf(pathToTk, "%s%s%s%s", sciPath, BASEPATHTOTHIRDPARTY, DIRECTORYOFTK, TCL_VERSION);

		char *exportTcl=(char*)MALLOC((strlen(EXPORTTCL)+strlen(pathToTcl)+1)*sizeof(char));
		char *exportTk=(char*)MALLOC((strlen(EXPORTTK)+strlen(pathToTk)+1)*sizeof(char));
		sprintf(exportTcl,"%s%s",EXPORTTCL,pathToTcl);
		sprintf(exportTk,"%s%s",EXPORTTK,pathToTk);
		/* Export both TCL_LIBRARY & TK_LIBRARY */
		putenv(exportTcl);
		putenv(exportTk);
	}
#endif
#endif

#endif
	/* set TMPDIR */
	C2F(settmpdir)();

	/* For the initialization of the Scilab primivite : fromjava() */
	SetFromJavaToON();

	InitializeLaunchScilabSignal();

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
/*--------------------------------------------------------------------------*/
