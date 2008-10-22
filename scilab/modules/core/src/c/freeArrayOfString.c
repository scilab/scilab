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
	BOOL bRet = TRUE;

	if (Str)
	{
		int i = 0;
		for (i = 0;i < dim; i++)
		{
			if (Str[i])
			{
				FREE(Str[i]);
				Str[i] = NULL;
			}
			else bRet = FALSE;
		}
		FREE(Str);
		Str = NULL;
		return bRet;
	}
	else
	{
		return FALSE;
	}
}
/*---------------------------------------------------------------------------*/ 
