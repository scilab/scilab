/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include <string.h>
#include "toolsdictionary.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
/**
* merge two strings array
* append array2 to array1
* @param[in] array1 
* @param[in] pos
* @param[in] array2
* @param[in] sizearray2
* @return string array updated;
*/
static char **mergearrays(char **array1,int pos,char **array2,int sizearray2);

/* compare function for qsort */
static int comparewords (const void * a, const void * b);
/*-----------------------------------------------------------------------------------*/ 
BOOL appendDictionary(char ***dictionary,int *i,char ***datas,int *sizedatas)
{
	BOOL bOK = FALSE;
	if (*datas)
	{
		*dictionary = mergearrays(*dictionary,*i,*datas,*sizedatas);
		*i = *i + *sizedatas;

		FREE(*datas);
		*datas = NULL;
		*sizedatas = 0;
		bOK = TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static char **mergearrays(char **array1,int pos,char **array2,int sizearray2)
{
	if (array1 && array2)
	{
		int j = 0;
		for (j = 0;j < sizearray2;j++)
		{
			array1[pos] = array2[j];
			pos++;
		}
		return array1;
	}
	return NULL;
}
/*-----------------------------------------------------------------------------------*/ 
static int comparewords (const void * a, const void * b)
{
	return ( strcmp(*(char**)a,*(char**)b) );
}
/*-----------------------------------------------------------------------------------*/ 
char ** SortDictionary(char **Strings,int SizeStrings)
{
	qsort(Strings,SizeStrings,sizeof(char*),comparewords);
	return Strings;
}
/*-----------------------------------------------------------------------------------*/
char ** RemoveDuplicateDictionary(char **Strings,int *SizeStrings)
{
	char **returnedArray = NULL;
	int newsize = 0;
	
	if (Strings)
	{
		int i = 0, j = 0;
		for (i = j = 0; i < *SizeStrings - 1; i++) 
		{
			if (strcmp(Strings[i], Strings[i + 1])) Strings[j++] = Strings[i];
			else 
			{
				FREE(Strings[i]);
				Strings[i] = NULL;
			}
		}
		Strings[j++] = Strings[i];
		newsize = j;

		returnedArray = Strings;
		if (returnedArray) *SizeStrings = newsize;
	}
	return returnedArray;
}
/*-----------------------------------------------------------------------------------*/ 
