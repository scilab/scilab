
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#include <ctype.h>
#include "isletter.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
BOOL *isletter(char *input_string, int *sizeArray)
{
	BOOL *returnedValues = NULL;
	if (input_string)
	{
		int i = 0;
		int length_input_string = (int)strlen(input_string);
		*sizeArray = length_input_string;

		if (length_input_string > 0)
		{
			returnedValues = (BOOL*)MALLOC(sizeof(BOOL)*length_input_string);
			if (returnedValues)
			{
				for (i = 0;i < length_input_string; i++)
				{
					if ( isalpha(input_string[i]) ) returnedValues[i] = TRUE;
					else returnedValues[i] = FALSE;
				}
			}
		}
	}
	return returnedValues;
}
/*--------------------------------------------------------------------------*/
BOOL *isletterW(wchar_t *wcInput_string, int *sizeArray)
{
	BOOL *returnedValues = NULL;
	if (wcInput_string)
	{
		int i = 0;
		int length_input_string = (int)wcslen(wcInput_string);

		*sizeArray = length_input_string;

		if (length_input_string > 0)
		{
			returnedValues = (BOOL*)MALLOC(sizeof(BOOL)*length_input_string);
			if (returnedValues)
			{
				for (i = 0;i < length_input_string; i++)
				{
					if ( iswalpha(wcInput_string[i]) ) returnedValues[i] = TRUE;
					else returnedValues[i] = FALSE;
				}
			}
		}
	}
	return returnedValues;
}
/*--------------------------------------------------------------------------*/
