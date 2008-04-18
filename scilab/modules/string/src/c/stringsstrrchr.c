
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

/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "stringsstrrchr.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*----------------------------------------------------------------------------*/
char ** strings_strrchr(char **InputStrings,int Dim_InputStrings,char** InputChar,int Dim_InputChar,BOOL do_strrchr)
{
	char **Output_Strings = NULL;

	if ( (InputStrings) && (InputChar) )
	{
		Output_Strings = (char**)MALLOC(sizeof(char*)*Dim_InputStrings);
		if (Output_Strings)
		{
			int i = 0;
			for(i = 0;i < Dim_InputStrings; i++)
			{
				int c = 0;
				char *ptrStr = NULL;

				if (Dim_InputChar == 1) c = InputChar[0][0];
				else  c = InputChar[i][0];

				if (do_strrchr) ptrStr = strrchr(InputStrings[i],c);
				else ptrStr = strchr(InputStrings[i],c);

				if (ptrStr)
				{
					Output_Strings[i] = strdup(ptrStr);
				}
				else
				{
					Output_Strings[i] = strdup("");
				}
			}
		}
	}
	return Output_Strings;
}
/*--------------------------------------------------------------------------*/
