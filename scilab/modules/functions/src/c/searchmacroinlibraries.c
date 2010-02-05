/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "searchmacroinlibraries.h"
#include "librarieslist.h"
#include "libraryinfo.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif 
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
char **searchmacroinlibraries(char *macro, int *sizeReturnedArray)
{
	char **returnedArray = NULL;
	*sizeReturnedArray = 0;

	if (macro)
	{
		int sizelibraries = 0;
		char **libraries = getlibrarieslist(&sizelibraries);
		if ( libraries && (sizelibraries > 0) )
		{
			int nbLibsWhereIsMacro = 0;
			int i = 0;

			for (i = 0; i < sizelibraries; i++)
			{
				int sizemacrosLib = 0;
				char **macrosLib = getlistmacrosfromlibrary(libraries[i], &sizemacrosLib);

				if (macrosLib && (sizemacrosLib > 0) )
				{
					int j = 0;
					for (j = 0; j < sizemacrosLib; j++)
					{
						if (strcmp(macrosLib[j], macro) == 0)
						{
							nbLibsWhereIsMacro++;
							if (nbLibsWhereIsMacro == 1)
							{
								returnedArray = (char**)MALLOC(sizeof(char*)* nbLibsWhereIsMacro);
							}
							else
							{
								returnedArray = (char**)REALLOC(returnedArray, sizeof(char*)*(nbLibsWhereIsMacro));
							}
							returnedArray[nbLibsWhereIsMacro - 1] = strdup(libraries[i]);
						}
					}
					freeArrayOfString(macrosLib, sizemacrosLib);
				}
			}
			*sizeReturnedArray = nbLibsWhereIsMacro;
			freeArrayOfString(libraries, sizelibraries);
		}
	}
	return returnedArray;
}
/*--------------------------------------------------------------------------*/
