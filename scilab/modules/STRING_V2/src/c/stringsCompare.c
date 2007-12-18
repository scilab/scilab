/*--------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stringsCompare.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#define stricmp _stricmp
#endif
/*--------------------------------------------------------------------------*/
int * stringsCompare(char **Input_String_One,int dim_One,char **Input_String_Two,int dim_Two,BOOL dostricmp)
{
	int *returnedValues = NULL;
	int j = 0;

	returnedValues = (int*)MALLOC(sizeof(int)*dim_One);
	if (returnedValues)
	{
		int i = 0;
		for (i = 0;i < dim_One; i++)
		{
			if (dim_Two == 1) j = 0;
			else j = i;

			if (dostricmp) 
			{
				#ifdef _MSC_VER
				returnedValues[i] = stricmp(Input_String_One[i],Input_String_Two[j]);
				#else
				returnedValues[i] = strcasecmp (Input_String_One[i],Input_String_Two[j]);
				#endif
			}
			else returnedValues[i] = strcmp(Input_String_One[i],Input_String_Two[j]);
		}
	}
	return returnedValues;
}
/*--------------------------------------------------------------------------*/
