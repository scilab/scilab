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

	returnedValues = (int*)MALLOC(sizeof(int)*dim_One);
	if (returnedValues)
	{
		if ( dim_Two == 1 )
		{
			int i = 0;
			for (i = 0;i < dim_One; i++)
			{
				if (dostricmp) returnedValues[i] = stricmp(Input_String_One[i],Input_String_Two[0]);
				else returnedValues[i] = strcmp(Input_String_One[i],Input_String_Two[0]);
			}
		}
		else
		{
			int i = 0;
			for (i = 0;i < dim_One; i++)
			{
				if (dostricmp) returnedValues[i] = stricmp(Input_String_One[i],Input_String_Two[i]);
				else returnedValues[i] = strcmp(Input_String_One[i],Input_String_Two[i]);
			}
		}
	}
	return returnedValues;
}
/*--------------------------------------------------------------------------*/
