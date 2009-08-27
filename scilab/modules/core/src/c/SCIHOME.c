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
#include "createdirectory.h"
#include "isdir.h"
#include "../../../io/includes/getenvc.h"
#include "scilabDefaults.h"
#ifdef _MSC_VER
#include "getshortpathname.h"
#include "strdup_Windows.h"
#endif
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
static char SCIHOMEPATH[PATH_MAX*2] = "empty_SCIHOME";
/*--------------------------------------------------------------------------*/
static BOOL defineSCIHOME(void);
/*--------------------------------------------------------------------------*/
BOOL setSCIHOME(void)
{
	int ierr = 0;
	int buflen = PATH_MAX;
	int iflag = 0;
	char SCIHOME[PATH_MAX];

	C2F(getenvc)(&ierr,"SCIHOME",SCIHOME,&buflen,&iflag);

	if (ierr) /* SCIHOME not define */
	{
		return defineSCIHOME();
	}
	else /* SCIHOME already defined */
	{
		strcpy(SCIHOMEPATH,SCIHOME);
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getSCIHOME(void)
{
	return strdup(SCIHOMEPATH);
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static BOOL defineSCIHOME(void)
{
	#define BASEDIR "Scilab"
	int ierr = 0;
	int buflen = PATH_MAX;
	int iflag = 0;

	char USERPATHSCILAB[PATH_MAX];

	char USERHOMESYSTEM[PATH_MAX];
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


	/* Set SCIHOME environment variable */
	sprintf(USERPATHSCILAB,"%s%s%s",USERHOMESYSTEM,DIR_SEPARATOR,BASEDIR);
	sprintf(SCIHOMEPATH,"%s%s%s",USERPATHSCILAB,DIR_SEPARATOR,SCI_VERSION_STRING);

	/* creates directory if it does not exists */
	if (!isdir(SCIHOMEPATH))
	{
		if(!isdir(USERPATHSCILAB)) createdirectory(USERPATHSCILAB);
		if (createdirectory(SCIHOMEPATH)) return TRUE;
	}
	else return TRUE;

	return FALSE;
}
#else
static BOOL defineSCIHOME(void)
{
	#define BASEDIR ".Scilab"
	int ierr   = 0;
	int buflen = PATH_MAX;
	int iflag  = 0;
	char USERPATHSCILAB[PATH_MAX];
	char USERHOMESYSTEM[PATH_MAX];

	C2F(getenvc)(&ierr,"HOME",USERHOMESYSTEM,&buflen,&iflag);
	if (ierr) return FALSE; 

	/* Set SCIHOME environment variable */
	sprintf(USERPATHSCILAB,"%s%s%s",USERHOMESYSTEM,DIR_SEPARATOR,BASEDIR);
	sprintf(SCIHOMEPATH,"%s%s%s",USERPATHSCILAB,DIR_SEPARATOR,SCI_VERSION_STRING);

	/* creates directory if it does not exists */
	if (!isdir(SCIHOMEPATH))
	{
		if(!isdir(USERPATHSCILAB)) createdirectory(USERPATHSCILAB);
		if (createdirectory(SCIHOMEPATH)) return TRUE;
	}
	else return TRUE;

	return FALSE;
}
#endif
/*--------------------------------------------------------------------------*/
