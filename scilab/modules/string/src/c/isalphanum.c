/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009
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
#include <wchar.h>
#include <wctype.h>
#include "isalphanum.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
BOOL *isalphanumW(wchar_t *input_string, int *returnedSize)
{
	BOOL *returnedValues = NULL;
	*returnedSize = 0;

	if (input_string)
	{
		int i = 0;
		int length_input_string = (int)wcslen(input_string);
		*returnedSize= length_input_string;

		if (length_input_string > 0)
		{
			returnedValues = (BOOL*)MALLOC(sizeof(BOOL)*length_input_string);
			if (returnedValues)
			{
				for (i = 0;i < length_input_string; i++)
				{
					if ( iswalnum(input_string[i]) ) returnedValues[i] = TRUE;
					else returnedValues[i] = FALSE;
				}
			}
		}
	}
	return returnedValues;
}
/*--------------------------------------------------------------------------*/
