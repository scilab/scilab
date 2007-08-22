/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "completion_generic.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
char **completion_generic(char **dictionary,int sizedictionary,
						  char *somechars, int *sizeArrayReturned)
{
	char **results = NULL;
	int nbElements = 0;
	int i = 0;

	for (i = 0;i < sizedictionary;i++)
	{
		if (dictionary[i])
		{
			if ( strncmp(dictionary[i],somechars,strlen(somechars)) == 0)
			{
				char *copybuf = NULL;

				nbElements++;
				if (results) results = (char**)REALLOC(results,sizeof(char*)*(nbElements));
				else results = (char**)MALLOC(sizeof(char*)*(nbElements));

				copybuf = (char*)MALLOC(sizeof(char)*(strlen(dictionary[i])+1));
				if (copybuf) strcpy(copybuf,dictionary[i]);
				results[nbElements-1] = copybuf;
			}
			else
			{
				if (nbElements > 0)
				{
					break;
				}
			}
		}
	}
	*sizeArrayReturned = nbElements;
	return results;
}
/*-----------------------------------------------------------------------------------*/
