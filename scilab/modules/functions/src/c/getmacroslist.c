/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include "getmacroslist.h"
#include "librarieslist.h"
#include "libraryinfo.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static int getsizemacroslist(void);
/*--------------------------------------------------------------------------*/
char **getmacroslist(int *sizearray)
{
	char **dictionary = NULL;
	int sizedictionary = getsizemacroslist();

	*sizearray = 0;

	if (sizedictionary > 0)
	{
		int sizelibraries = 0;
		char **libraries = getlibrarieslist(&sizelibraries);

		dictionary = (char**)MALLOC(sizeof(char*)*sizedictionary);

		if (dictionary)
		{
			int m = 0;
			if (libraries)
			{
				int i = 0;
				for (i = 0;i < sizelibraries; i++)
				{
					int j = 0;
					char **macros = NULL;
					int sizemacros = 0;

					macros = getlistmacrosfromlibrary(libraries[i],&sizemacros);

					if (macros)
					{
						for (j=0;j<sizemacros;j++)
						{
							dictionary[m] = macros[j];
							m++;
						}
						FREE(macros);
						macros = NULL;
					}
				}
				*sizearray = sizedictionary;
			}
		}
		else
		{
			*sizearray = 0;
		}
		freeArrayOfString(libraries, sizelibraries);
	}
	return dictionary;
}
/*--------------------------------------------------------------------------*/
static int getsizemacroslist(void)
{
	int sizedictionary = 0;
	int sizelibraries = 0;
	char **libraries = getlibrarieslist(&sizelibraries);

	if (libraries)
	{
		int i = 0;
		for (i = 0;i < sizelibraries; i++)
		{
			if (libraries[i])
			{
				char **macros = NULL;
				int sizemacros = 0;

				macros = getlistmacrosfromlibrary(libraries[i],&sizemacros);
				if (macros)
				{
					sizedictionary = sizedictionary + sizemacros;

					freeArrayOfString(macros, sizemacros);
				}

				FREE(libraries[i]);
				libraries[i] = NULL;
			}
			
		}
		FREE(libraries);
		libraries = NULL;
	}
	return sizedictionary;
}
/*--------------------------------------------------------------------------*/
