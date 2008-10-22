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
#include "PATH_MAX.h"
#include "../../../fileio/includes/createdirectory.h"
#include "../../../fileio/includes/isdir.h"
#include "../../../io/includes/getenvc.h"
#include "scilabDefaults.h"
#ifdef _MSC_VER
#include "../../../fileio/src/c/getshortpathname.h"
#include "strdup_Windows.h"
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#undef putenv
#define putenv(x) _putenv(x)
#endif
/*--------------------------------------------------------------------------*/
static char SCIHOMEPATH[PATH_MAX*2] = "empty_SCIHOME";
/*--------------------------------------------------------------------------*/
BOOL setSCIHOME(void)
{
	int ierr = 0;
	int buflen = PATH_MAX;
	int iflag = 0;
	
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

		char env[PATH_MAX+1+10]; /* PATH_MAX + strlen '\0' + strlen "SCIHOME=%s" */
		char USERHOMESYSTEM[PATH_MAX];

		iflag = 0;

		#ifdef _MSC_VER
		{
			char *SHORTUSERHOMESYSTEM = NULL;
			BOOL bConverted = FALSE;
			
			C2F(getenvc)(&ierr,"APPDATA",USERHOMESYSTEM,&buflen,&iflag);

			/* if APPDATA not found we try with USERPROFILE */
			if (ierr) C2F(getenvc)(&ierr,"USERPROFILE",USERHOMESYSTEM,&buflen,&iflag);

			/* convert long path to short path format : remove some special characters */
			SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM,&bConverted);
			if (SHORTUSERHOMESYSTEM)
			{
				if (!isdir(SHORTUSERHOMESYSTEM)) 
				{
					/* last chance, we try to get default all users profile */
					C2F(getenvc)(&ierr,"ALLUSERSPROFILE",USERHOMESYSTEM,&buflen,&iflag);
					if (ierr) 
					{
						FREE(SHORTUSERHOMESYSTEM); SHORTUSERHOMESYSTEM = NULL;
						return FALSE;
					}

					/* convert long path to short path format : remove some special characters */
					SHORTUSERHOMESYSTEM = getshortpathname(USERHOMESYSTEM,&bConverted);

					if ( (!SHORTUSERHOMESYSTEM) || (!isdir(SHORTUSERHOMESYSTEM)) )
					{
						if (SHORTUSERHOMESYSTEM) { FREE(SHORTUSERHOMESYSTEM); SHORTUSERHOMESYSTEM = NULL; }
						return FALSE;
					}
				}
			}
			else
			{
				if (SHORTUSERHOMESYSTEM) { FREE(SHORTUSERHOMESYSTEM); SHORTUSERHOMESYSTEM = NULL; }
				return FALSE;
			}

			/* checks that directory exists */
			strcpy(USERHOMESYSTEM,SHORTUSERHOMESYSTEM);
			if (SHORTUSERHOMESYSTEM) { FREE(SHORTUSERHOMESYSTEM); SHORTUSERHOMESYSTEM = NULL; }
		}
		#else /* Linux */
			C2F(getenvc)(&ierr,"HOME",USERHOMESYSTEM,&buflen,&iflag);
			if (ierr) return FALSE; 
		#endif

		/* Set SCIHOME environment variable */
		sprintf(USERPATHSCILAB,"%s%s%s",USERHOMESYSTEM,DIR_SEPARATOR,BASEDIR);
		sprintf(SCIHOMEPATH,"%s%s%s",USERPATHSCILAB,DIR_SEPARATOR,SCI_VERSION_STRING);
		sprintf(env,"SCIHOME=%s",SCIHOMEPATH);
		putenv(env);
	}
	else /* SCIHOME already defined */
	{
		strcpy(SCIHOMEPATH,SCIHOME);
	}

	/* creates directory if it does not exists */
	if (!isdir(SCIHOMEPATH))
	{
		if(!isdir(USERPATHSCILAB)) createdirectory(USERPATHSCILAB);
		if (createdirectory(SCIHOMEPATH)) return TRUE;
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
