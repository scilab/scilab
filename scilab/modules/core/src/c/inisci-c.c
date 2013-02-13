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
#include "getenvc.h"
#include "FileExist.h"
#include "localization.h"
#include "core_math.h" 
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "inisci-c.h"
#include "SCIHOME.h"
#include "scilabmode.h"
#include "PATH_MAX.h"
#include "scilabDefaults.h"
/*--------------------------------------------------------------------------*/
/**
 * Set the SCI path and initialize the scilab environement path
 *
 */
int SetSci(void)
{
	int ierr,iflag=0;
	int lbuf=PATH_MAX;
	char *buf = (char*)MALLOC(PATH_MAX*sizeof(char));
	if (buf)
	{
		C2F(getenvc)(&ierr,"SCI",buf,&lbuf,&iflag);

		if (ierr != 0)
		{
#ifdef  _MSC_VER
		    MessageBox(NULL,gettext("SCI environment variable not defined.\n"),gettext("Warning"),MB_ICONWARNING);
#else
		    fprintf(stderr, "%s", _("SCI environment variable not defined.\n"));
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
	char *pathtmp = NULL;

	SetSci();

	pathtmp = getSCIpath();
	if (pathtmp)
	{
		strcpy(buf,pathtmp);
		*nbuf = (int)strlen(buf);
		FREE(pathtmp);
		pathtmp = NULL;
	}
	else
	{
		*buf = NULL;
		*nbuf = 0;
	}
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
				MessageBox(NULL,gettext("SCIHOME not defined.\n"),gettext("Warning"),MB_ICONWARNING);
			#else
				fprintf(stderr,"%s",_("SCIHOME not defined.\n"));
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

	if (SCIHOME) {FREE(SCIHOME);SCIHOME=NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(gettmpdir)(char *buf,int *nbuf,long int lbuf)
{
	int ierr,iflag=0,l1buf=lbuf;
	C2F(getenvc)(&ierr,"TMPDIR",buf,&l1buf,&iflag);
	if (ierr != 0) 
	{
#ifdef  _MSC_VER
		MessageBox(NULL,gettext("TMPDIR not defined.\n"),gettext("Warning"),MB_ICONWARNING);
#else
		fprintf(stderr,"%s",_("TMPDIR not defined.\n"));
#endif
		exit(1);
	}

	*nbuf = (int)strlen(buf);
	return 0;
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
	*defaultsize = DEFAULTGSTACKSIZE;
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(getdefaultstacksize)(int *defaultsize)
{
	*defaultsize = DEFAULTSTACKSIZE;
	return 0;
}
/*--------------------------------------------------------------------------*/
