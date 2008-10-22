
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
#include "MALLOC.h"
#include "partfunction.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
#define BLANK_CHAR ' '
/*--------------------------------------------------------------------------*/
char **partfunction(char** stringInput,int m,int n,int *vectInput,int row)
{
	char **parts = NULL;
	int mn = m * n;

	parts = (char**)MALLOC(sizeof(char*)*(mn));
	if (parts)
	{
		int i = 0;
		for (i = 0;i < mn; i++)
		{
			int j = 0;
			int lengthstringInput = (int)strlen(stringInput[i]);

			parts[i] = (char*)MALLOC(sizeof(char)*((row)+1));
			if (parts[i] == NULL)
			{
				freeArrayOfString(parts,mn);
				return NULL;
			}

			for (j = 0;j < row; j++)
			{
				if ( vectInput[j] > lengthstringInput )
				{
					parts[i][j] = BLANK_CHAR;
				}
				else
				{
					parts[i][j] = stringInput[i][vectInput[j]-1];
				}
			}
			parts[i][j] ='\0';
		}
	}
	return parts;
}
/*--------------------------------------------------------------------------*/
