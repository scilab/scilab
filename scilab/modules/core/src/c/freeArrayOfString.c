/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include "freeArrayOfString.h"
#include "MALLOC.h"
/*---------------------------------------------------------------------------*/ 
BOOL freeArrayOfString(char **Str,int dim)
{
	return freeArray((void**)Str, dim);
}
/*---------------------------------------------------------------------------*/ 
BOOL freeArrayOfWideString(wchar_t **wcStr, int dim)
{
	return freeArray((void**)wcStr, dim);
}
/*---------------------------------------------------------------------------*/ 
BOOL freeArray(void **pArray, int dim)
{
	BOOL bRet = TRUE;

	if (pArray)
	{
		int i = 0;
		for (i = 0;i < dim; i++)
		{
			if (pArray[i])
			{
				FREE(pArray[i]);
				pArray[i] = NULL;
			}
			else bRet = FALSE;
		}
		FREE(pArray);
		pArray = NULL;
		return bRet;
	}
	else
	{
		return FALSE;
	}
}
/*---------------------------------------------------------------------------*/ 