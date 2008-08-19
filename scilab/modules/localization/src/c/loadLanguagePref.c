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
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "LanguagePref.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
char *loadLanguagePref(void)
{
	char *filepref = getFilenameLanguagePref();

	if (filepref)
	{
		char *LANGUAGE = NULL;

		FILE *fileR = fopen(filepref, "rt");

		if (fileR)
		{
			char LINE[PATH_MAX];

			fscanf(fileR, "%[^\n]", LINE);
			LANGUAGE = strdup(LINE);

			fclose(fileR);
		}

		FREE(filepref);
		filepref = NULL;

		if (LANGUAGE) return LANGUAGE;
	}
	/* the "" means that we will try to use the language of the system. */
	return strdup("");
}
/*--------------------------------------------------------------------------*/
