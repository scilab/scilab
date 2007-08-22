/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "getmacrosdictionary.h"
#include "librarieslist.h"
#include "libraryinfo.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
static int getsizemacrosdictionary(void);
/*-----------------------------------------------------------------------------------*/
char **getmacrosdictionary(int *sizearray)
{
	int sizelibraries = 0;
	char **libraries = getlibrarieslist(&sizelibraries);
	char **dictionary = NULL;
	int sizedictionary = getsizemacrosdictionary();

	*sizearray = 0;

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

	if (libraries)
	{
		int i = 0;
		for (i = 0;i < sizelibraries; i++)
		{
			if (libraries[i])
			{
				FREE(libraries[i]);
				libraries[i] = NULL;
			}
		}
		FREE(libraries);
		libraries = NULL;
	}

	return dictionary;
}
/*-----------------------------------------------------------------------------------*/
static int getsizemacrosdictionary(void)
{
	int sizedictionary = 0;
	int sizelibraries = 0;
	char **libraries = getlibrarieslist(&sizelibraries);

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
				sizedictionary = sizedictionary + sizemacros;

				for (j = 0;j < sizemacros; j++)
				{
					if(macros[j])
					{
						FREE(macros[j]);
						macros[j] = NULL;
					}
				}
				FREE(macros);
				macros = NULL;
			}

			FREE(libraries[i]);
			libraries[i] = NULL;
		}
		FREE(libraries);
		libraries = NULL;
	}
	return sizedictionary;
}
/*-----------------------------------------------------------------------------------*/
