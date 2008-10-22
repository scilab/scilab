
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

/* desc : str=code2str(c)
          Returns character string associated with Scilab int codes.
          str is such that c(i) is the Scilab int code of part(str,i))*/
/*------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "code2str.h"
#include "getfastcode.h"
/*--------------------------------------------------------------------------*/
void code2str(char **Output_Matrix, int *Input_Matrix,int Dim_Input_Matrix)
{
    int x = 0;
	/* To look up in the table */
	for ( x = 0;x < Dim_Input_Matrix; x++)                     
	{
		Output_Matrix[0][x] = convertScilabCodeToAsciiCode(Input_Matrix[x]);
	}
}
/*--------------------------------------------------------------------------*/
void str2code(int *Output_Matrix, char **Input_String)
{
	int i = 0;
	int nbOutput_Matrix = (int)strlen(Input_String[0]);

	for (i = 0;i < nbOutput_Matrix ;i++)
    {
		Output_Matrix[i] = convertAsciiCodeToScilabCode(Input_String[0][i]);
    }
}
/*--------------------------------------------------------------------------*/
