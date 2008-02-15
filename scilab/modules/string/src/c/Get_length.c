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
/* desc : Caculate the length of the output string                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "Get_length.h"
/*------------------------------------------------------------------------*/
int Get_length(char **Input_String_One,char **Input_String_Two,int Number_Input_One)
{
    int length = 0;
	int i = 0;

	for (i = 0; i < Number_Input_One; i++)    
	{  
		length = length + (int)strlen(Input_String_One[i]) + (int)strlen(Input_String_Two[0]);
	}
	length = length - (int)strlen(Input_String_Two[0]);
	return length;
}
/*------------------------------------------------------------------------*/
