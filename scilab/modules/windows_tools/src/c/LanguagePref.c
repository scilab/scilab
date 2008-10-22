/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "SCIHOME.h"
#include "MALLOC.h"
#include "LanguagePref.h"
/*--------------------------------------------------------------------------*/
#if _MSC_VER
	#define namelanguagepref "\\language.ini"
#else
	#define namelanguagepref "/.language"
#endif
/*--------------------------------------------------------------------------*/
char *getFilenameLanguagePref(void)
{
	char *retFilename = NULL;
	char *SCIHOME = NULL;

	SCIHOME = getSCIHOME();

	if (SCIHOME)
	{
		retFilename = (char*)MALLOC( (strlen(namelanguagepref) + strlen(SCIHOME) + 1) *sizeof(char));
		sprintf(retFilename,"%s%s",SCIHOME,namelanguagepref);
		FREE(SCIHOME);
		SCIHOME = NULL;
	}
	else
	{
		retFilename = (char*)MALLOC((strlen(namelanguagepref) + 1)*sizeof(char));
		strcpy(retFilename,namelanguagepref);
	}

	return retFilename;
}
/*--------------------------------------------------------------------------*/
