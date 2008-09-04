/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SCIHOME.h"
#include "MALLOC.h"
#include "version.h"
#include "machine.h"

#include "../../../fileio/includes/createdirectory.h"
#include "../../../fileio/includes/isdir.h"
#include "../../../io/includes/getenvc.h"
#include "scilabDefaults.h"
#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif
/*--------------------------------------------------------------------------*/
#if (defined _MSC_VER) 
#undef putenv
#define putenv(x) _putenv(x)
#endif
/*--------------------------------------------------------------------------*/
static char SCIHOMEPATH[PATH_MAX*2]="empty_SCIHOME";
/*--------------------------------------------------------------------------*/
BOOL setSCIHOME(void)
{
	int ierr=0;
	int buflen=PATH_MAX;
	int iflag=0;
	
	char SCIHOME[PATH_MAX];
	char USERPATHSCILAB[PATH_MAX];
	
	C2F(getenvc)(&ierr,"SCIHOME",SCIHOME,&buflen,&iflag);

	if (ierr) /* SCIHOME not define */
	{
		#ifdef _MSC_VER
			#define BASEDIR "Scilab"
		#else
			#define BASEDIR ".Scilab"
		#endif

		char env[PATH_MAX+1+10];
		char USERHOMESYSTEM[PATH_MAX];
		iflag = 0;

		#ifdef _MSC_VER
			C2F(getenvc)(&ierr,"APPDATA",USERHOMESYSTEM,&buflen,&iflag);
			if (ierr) C2F(getenvc)(&ierr,"USERPROFILE",USERHOMESYSTEM,&buflen,&iflag);
			/* checks that directory exists */
			if (!isdir(USERHOMESYSTEM)) 
			{
				/* last chance, we try to get default all users profile */
				C2F(getenvc)(&ierr,"ALLUSERSPROFILE",USERHOMESYSTEM,&buflen,&iflag);
				if ( (ierr) || (!isdir(USERHOMESYSTEM)) ) return FALSE;
			}
		#else
			C2F(getenvc)(&ierr,"HOME",USERHOMESYSTEM,&buflen,&iflag);
		#endif

		if (ierr) 
		{
			return FALSE; 
		}
		else
		{
			sprintf(USERPATHSCILAB,"%s%s%s",USERHOMESYSTEM,DIR_SEPARATOR,BASEDIR);
			sprintf(SCIHOMEPATH,"%s%s%s",USERPATHSCILAB,DIR_SEPARATOR,SCI_VERSION_STRING);
		}
		sprintf(env,"SCIHOME=%s",SCIHOMEPATH);
		putenv(env);
	}
	else
	{
		strcpy(SCIHOMEPATH,SCIHOME);
	}

	if (!isdir(SCIHOMEPATH))
	{
		if(!isdir(USERPATHSCILAB)) createdirectory(USERPATHSCILAB);
		if (createdirectory(SCIHOMEPATH))
		{
			return TRUE;
		}
	}
	else return TRUE;
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getSCIHOME(void)
{
	return strdup(SCIHOMEPATH);
}
/*--------------------------------------------------------------------------*/
