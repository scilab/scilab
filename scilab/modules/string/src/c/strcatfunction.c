/*------------------------------------------------------------------------*/
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

/* desc : conconcatenate: character strings                                  */
/*        The main function                                               */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "strcatfunction.h"
/*----------------------------------------------------------------------------*/
BOOL strcatfunction(char **Input_String_One,char **Input_String_Two,int Number_Input_One,char **Output_StringMatrix)
{
	int x = 0;

	if (Number_Input_One > 1)
	{
		/* Travers for every argument*/
		for (x = 1; x < (Number_Input_One-1) ;++x)
		{
			if (strlen(Input_String_Two[0]) == 0) return FALSE;

			/* Put the each of the two argument together */
			strcat(Output_StringMatrix[0],Input_String_One[x]);
			strcat(Output_StringMatrix[0] ,Input_String_Two[0]);
		}
		strcat(Output_StringMatrix[0] ,Input_String_One[Number_Input_One-1]);
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
