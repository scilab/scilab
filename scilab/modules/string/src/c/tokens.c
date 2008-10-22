
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - 2008 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "MALLOC.h" 
#include "tokens.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*------------------------------------------------------------------------*/
char** stringTokens(char *str,char **delim,int sizedelim,int *sizeOutputs)
{
	char **Outputs = NULL;
	*sizeOutputs = 0;

	if (str)
	{
		char *delimiters = NULL;
		int i = 0;
		char *token = NULL;

		delimiters = (char*)MALLOC(sizeof(char)*(sizedelim + 1));
		if (delimiters)
		{
			for (i = 0;i < sizedelim; i++)
			{
				delimiters[i] = delim[i][0];
			}
			delimiters[i] = 0;

			token = strtok(str,delimiters);

			while ( token != NULL)
			{
				(*sizeOutputs)++; 
				if (Outputs == NULL)
				{
					Outputs = (char**)MALLOC(sizeof(char*)*(*sizeOutputs));
				}
				else
				{
					Outputs = (char**)REALLOC(Outputs,sizeof(char*)*(*sizeOutputs));
				}
				Outputs[*sizeOutputs - 1] = strdup(token);
				token = strtok(NULL,delimiters);
			}
			FREE(delimiters);
			delimiters = NULL;
		}
	}
	return Outputs;
}
/*--------------------------------------------------------------------------*/
