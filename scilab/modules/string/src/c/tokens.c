
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
#include "charEncoding.h"
/*------------------------------------------------------------------------*/
char** stringTokens(char *str,char **delim,int sizedelim,int *sizeOutputs)
{
	char **Outputs = NULL;
	*sizeOutputs = 0;

	if (str)
	{
		wchar_t *wcdelimiters = NULL;
		wcdelimiters = (wchar_t*)MALLOC(sizeof(wchar_t)*(sizedelim + 1));

		if (wcdelimiters)
		{
			int i = 0;
			wchar_t * wcstr = to_wide_string(str);
			wchar_t *wctoken = NULL;
			#ifndef _MSC_VER
			wchar_t *state = NULL;
			#endif

			for (i = 0;i < sizedelim; i++)
			{
				wchar_t * wcdelim = to_wide_string(delim[i]);
				wcdelimiters[i] = wcdelim[0];
				FREE(wcdelim); wcdelim = NULL;
			}
			wcdelimiters[i] = L'\0';

			#ifndef _MSC_VER
			wctoken = wcstok(wcstr,wcdelimiters,&state);
			#else
			wctoken = wcstok(wcstr,wcdelimiters);
			#endif

			while ( wctoken != NULL)
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
				Outputs[*sizeOutputs - 1] = wide_string_to_UTF8(wctoken);
				#ifndef _MSC_VER
				wctoken =  wcstok(NULL,wcdelimiters,&state);
				#else
				wctoken =  wcstok(NULL,wcdelimiters);
				#endif
			}

			if (wcdelimiters) {FREE(wcdelimiters);	wcdelimiters = NULL;}
			if (wcstr) {FREE(wcstr);wcstr = NULL;}
		}
		
	}
	return Outputs;
}
/*--------------------------------------------------------------------------*/
