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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "InitializeJVM.h"
#include "loadClasspath.h"
#include "loadLibrarypath.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "JVM.h"
#include "createMainScilabObject.h"
#include "scilabDefaults.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
static void DoLoadClasspathInEtc(char *sciPath);
static void DoLoadLibrarypathInEtc(char *sciPath);
/*--------------------------------------------------------------------------*/ 
BOOL InitializeJVM(void)
{
	BOOL bOK=FALSE;
	char *sciPath=NULL;

	sciPath=getSCIpath();

	if (!startJVM(sciPath))
	{
#ifdef _MSC_VER
		MessageBox(NULL,gettext("\nScilab cannot open JVM library.\n"),gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
		fprintf(stderr,_("\nScilab cannot open JVM library.\n"));
#endif
	}
	else
	{
		DoLoadLibrarypathInEtc(sciPath);
		DoLoadClasspathInEtc(sciPath);

		if (!createMainScilabObject())
		{
#ifdef _MSC_VER
			MessageBox(NULL,gettext("\nScilab cannot create Scilab Java Main-Class (we have not been able to find the main Scilab class. Check if the Scilab and thirdparty packages are available).\n"),gettext("Error"),MB_ICONEXCLAMATION|MB_OK);
#else
			fprintf(stderr,_("\nScilab cannot create Scilab Java Main-Class (we have not been able to find the main Scilab class. Check if the Scilab and thirdparty packages are available).\n"));
#endif
		}
		else
			{
				bOK=TRUE;
			}
	}

	if (sciPath) {FREE(sciPath);sciPath=NULL;}

	if (!bOK) exit(1);

	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
static void DoLoadClasspathInEtc(char *sciPath)
{
	char *classpathfile = (char*)MALLOC(sizeof(char)*(strlen(sciPath)+strlen(XMLCLASSPATH)+1));
	sprintf(classpathfile,XMLCLASSPATH,sciPath);
	LoadClasspath(classpathfile);
	if (classpathfile) {FREE(classpathfile); classpathfile = NULL;}
}
/*--------------------------------------------------------------------------*/ 
static void DoLoadLibrarypathInEtc(char *sciPath)
{
	char *librarypathfile = (char*)MALLOC(sizeof(char)*(strlen(sciPath)+strlen(XMLLIBRARYPATH)+1));
	sprintf(librarypathfile,XMLLIBRARYPATH,sciPath);
	LoadLibrarypath(librarypathfile);
	if (librarypathfile) {FREE(librarypathfile); librarypathfile = NULL;}
}
/*--------------------------------------------------------------------------*/ 


