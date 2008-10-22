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
#include <stdlib.h>
#include <string.h>
#include "saveLanguagePref.h"
#include "LanguagePref.h"
#include "deleteafile.h"
#include "setgetlanguage.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
BOOL saveLanguagePref(void)
{
	BOOL bOK = FALSE;
	char *filepref = getFilenameLanguagePref();

	if (filepref)
	{
		FILE *fileW = fopen(filepref, "wt");
		if (fileW)
		{
			char *LANGUAGE = getlanguage();

			if (LANGUAGE)
			{
				fputs(LANGUAGE,fileW);
				fputs("\n",fileW);
				fclose(fileW);
				bOK = TRUE;
			}
			else
			{
				fclose(fileW);
				deleteafile(filepref);
				bOK = FALSE;
			}

			FREE(filepref);
			filepref = NULL;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
