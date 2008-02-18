
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

/* desc : returns the tokens of a character string.                       */
/*        The main function                                               */
/*		                                                                  */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "MALLOC.h" 
#include "tokens.h"
/*------------------------------------------------------------------------*/
void tokens(char **Input_MatrixOne,char **Input_MatrixTwo,char **Output_String,int *Row_Pointer,int *Col_Pointer,int Number_Input_One,int Number_Input_Two)
{
	int x = 0;
	
	for (x = 0; x < Number_Input_One; x++)
	{
		int y = 0;

		/*To loop on every string in the input string matrix */
		for (y = 0; y < (int)strlen(Input_MatrixOne[x]); y++)
		{
			int i = 0;
			int bo = 0;

			for (i=0; i < Number_Input_Two; i++)
			{
				if ( Input_MatrixOne[x][y] == Input_MatrixTwo[i][0] ) bo = 1;
			}
			if ( bo == 0 )  
			{
				/* if bo is a signal, to put the input string matrix into the output matrix in a correct token way*/
				Output_String[*Row_Pointer][(*Col_Pointer)++] = Input_MatrixOne[0][y];
			}
			else 
			{
				/* To start copying from the next row, and from the very beginning*/
				if ((*Col_Pointer)!= 0) 
				{
					Output_String[*Row_Pointer][*Col_Pointer] = 0;
					(*Row_Pointer)++;
					*Col_Pointer = 0;
				}
			}
			bo = 0;
		}
	}
    Output_String[*Row_Pointer][*Col_Pointer] = 0;
}
/*------------------------------------------------------------------------*/
