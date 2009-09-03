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
/*----------------------------------------------------------------------------*/
#include "strsplitfunction.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
wchar_t **strsplitfunction(wchar_t * wcstringToSplit, double *indices, int sizeIndices, strsplit_error *ierr)
{
	wchar_t **splitted = NULL;
	*ierr = STRSPLIT_NO_ERROR;

	if (wcstringToSplit)
	{
		int lengthToCopy = 0;
		int lenString = (int)wcslen(wcstringToSplit);
		int i = 0;
		int j = 0;

		/* Check 2nd input matrix position */
		for (i = 0; i < sizeIndices; i++)
		{
			if ( ((int)indices[i] <= 0) || ((int)indices[i] >= lenString) )
			{
				*ierr = STRSPLIT_INCORRECT_VALUE_ERROR;
				return NULL;
			}
		}

		/* check 2nd input order */
		if (sizeIndices > 1)
		{
			for (i = 0; i < sizeIndices; i++)
			{
				if ( i < (sizeIndices - 1) )
				{
					if ((int)indices[i] > (int)indices[i+1])
					{
						*ierr = STRSPLIT_INCORRECT_ORDER_ERROR;
						return NULL;
					}
				}
			}
		}

		splitted = (wchar_t**)MALLOC(sizeof(wchar_t*) * (sizeIndices + 1));
		if (splitted == NULL)
		{
			*ierr = STRSPLIT_MEMORY_ALLOCATION_ERROR;
			return NULL;
		}

		for (i = 0; i < sizeIndices; i++)
		{
			lengthToCopy = 0;

			if (i == 0)
			{
				lengthToCopy = (int)indices[i];
			}
			else
			{
				lengthToCopy = (int)indices[i] - (int)indices[i-1];
			}

			splitted[i] = (wchar_t*)MALLOC(sizeof(wchar_t)*(lengthToCopy + 1));
			if (splitted[i] == NULL)
			{
				freeArrayOfWideString(splitted, sizeIndices);
				*ierr = STRSPLIT_MEMORY_ALLOCATION_ERROR;
				return NULL;
			}

			wcsncpy(splitted[i], &wcstringToSplit[j], lengthToCopy);

			j = (int)indices[i];
		}

		lengthToCopy = lenString - (int)indices[sizeIndices - 1];
		splitted[sizeIndices] = (wchar_t*)MALLOC(sizeof(wchar_t)*(lengthToCopy + 1));
		if (splitted[sizeIndices] == NULL)
		{
			freeArrayOfWideString(splitted, sizeIndices + 1);
			*ierr = STRSPLIT_MEMORY_ALLOCATION_ERROR;
			return NULL;
		}

		wcsncpy(splitted[sizeIndices], &wcstringToSplit[j], lengthToCopy);
	}
	return splitted;
}
/*----------------------------------------------------------------------------*/
