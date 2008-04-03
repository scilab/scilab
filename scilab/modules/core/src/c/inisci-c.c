/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include <windows.h>
#else
#include <sys/utsname.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* getenv ! */
#include "../../../io/includes/getenvc.h"
#include "../../../fileio/includes/FileExist.h"
#include "localization.h"
#include "machine.h" 
#include "Os_specific.h"
#include "core_math.h" 
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "inisci-c.h"
#include "SCIHOME.h"
#include "scilabmode.h"
#include "scilabDefaults.h"
/*--------------------------------------------------------------------------*/
int C2F(withmodelicac)(int *rep)
{
	if (ExistModelicac())
	{
		*rep =1;
	}
	else
	{
		*rep =0; 
	}
  return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(withmsdos)(int *rep)
{
#ifdef _MSC_VER
	*rep =1;
#else
	*rep =0; 
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(getcomp)(char *buf,int *nbuf,long int lbuf)
{
  int ierr,iflag=0,l1buf=lbuf;
  C2F(getenvc)(&ierr,"COMPILER",buf,&l1buf,&iflag);
  if ( ierr== 1) strncpy(buf,"NO",lbuf);
  *nbuf = (int)strlen(buf);
  return 0;
}
/*--------------------------------------------------------------------------*/
/**
 * Set the SCI path and initialize the scilab environement path
 *
 */
int SetSci()
{
	int ierr,iflag=0;
	int lbuf=PATH_MAX;
	char *buf = MALLOC(PATH_MAX*sizeof(char));
	if (buf)
	{
		C2F(getenvc)(&ierr,"SCI",buf,&lbuf,&iflag);

		if ( ierr== 1) 
		{
		#ifdef  _MSC_VER
		MessageBox(NULL,_("SCI environment variable not defined.\n"),"Warning",MB_ICONWARNING);
		#else
		fprintf(stderr, _("SCI environment variable not defined.\n"));
		#endif
		exit(1);
		}
		setSCIpath(buf);
		FREE(buf);
		buf = NULL;
	}
	
	return 0;
}
/*--------------------------------------------------------------------------*/
/**
 * Get the SCI path and initialize the scilab environment path
 *
 */
int C2F(getsci)(char *buf,int *nbuf,long int lbuf)
{
	char *pathtmp=NULL;
	SetSci();
	pathtmp=getSCIpath();
	strcpy(buf,pathtmp);
	*nbuf = (int)strlen(buf);
	if (pathtmp) {FREE(pathtmp);pathtmp=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
/**
* Get the SCIHOME path and initialize the scilab environment path
*
*/
int C2F(getscihome)(char *buf,int *nbuf,long int lbuf)
{
	char *pathtmp=NULL;
	char *SCIHOME = getSCIHOME();
	if (strcmp(SCIHOME,"empty_SCIHOME")==0)
	{
		if (!setSCIHOME())
		{
			#ifdef  _MSC_VER
				MessageBox(NULL,_("SCIHOME not defined.\n"),_("Warning"),MB_ICONWARNING);
			#else
				/* @TODO change that to the error output */
				fprintf(stderr,_("SCIHOME not defined.\n"));
			#endif
			exit(1);
		}
		else
		{
			if (SCIHOME) {FREE(SCIHOME);SCIHOME=NULL;}
		}
	}

	pathtmp = getSCIHOME();
	if (pathtmp)
	{
		strcpy(buf,pathtmp);
		*nbuf = (int)strlen(buf);
		FREE(pathtmp);
		pathtmp = NULL;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(gettmpdir)(char *buf,int *nbuf,long int lbuf)
{
	int ierr,iflag=0,l1buf=lbuf;
	C2F(getenvc)(&ierr,"TMPDIR",buf,&l1buf,&iflag);
	if ( ierr== 1) 
	{
#ifdef  _MSC_VER
		MessageBox(NULL,_("TMPDIR not defined.\n"),_("Warning"),MB_ICONWARNING);
#else
		fprintf(stderr,_("TMPDIR not defined.\n"));
#endif
		exit(1);
	}
	*nbuf = (int)strlen(buf);
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL ExistModelicac(void)
{
	BOOL bOK=FALSE;
	char *SCIPATH = (char*)getSCIpath();
	char *fullpathModelicac=NULL;
	
	fullpathModelicac=(char*)MALLOC((strlen(SCIPATH)+strlen(ModelicacName)+1)*sizeof(char));
	sprintf(fullpathModelicac,"%s%s",SCIPATH,ModelicacName);

	bOK=FileExist(fullpathModelicac);
	if (fullpathModelicac) FREE(fullpathModelicac);
	if (SCIPATH) FREE(SCIPATH);
	return bOK;
}
/*--------------------------------------------------------------------------*/
int C2F(withgui)(int *rep)
{ 
	*rep = (getScilabMode() != SCILAB_NWNI); 
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(getdefaultgstacksize)(int *defaultsize)
{
#if defined(_MSC_VER) && defined(_WIN64)
	*defaultsize = DEFAULTGSTACKSIZE;
#else
	*defaultsize = DEFAULTGSTACKSIZE;
#endif
	
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(getdefaultstacksize)(int *defaultsize)
{
	*defaultsize = DEFAULTSTACKSIZE;
	return 0;
}
/*--------------------------------------------------------------------------*/
