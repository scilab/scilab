/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
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
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "CreateEmptystr.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
char ** CreateEmptystr(int m1,int n1)
{
	int m1n1 = m1*n1;
	char **OutputStrings = (char**)MALLOC(sizeof(char*)*(m1n1+1)); 

	if (OutputStrings)
	{
		int i = 0;
		for (i = 0;i < m1n1; i++)  /*m1 is the number of row ; n1 is the number of col*/
		{
			OutputStrings[i] = (char*)MALLOC(sizeof(char)*(strlen(EMPTY_STRING)+1));
			if (OutputStrings[i])
			{
				strcpy(OutputStrings[i],EMPTY_STRING);
			}
			else
			{
				freeArrayOfString(OutputStrings,m1n1);
				return NULL;
			}
		}
	}
	return OutputStrings;
}
/*--------------------------------------------------------------------------*/
