
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
#include <ctype.h> 
#include "gw_string.h"
#include "convstr.h"
/*--------------------------------------------------------------------------*/
void convstr(char **Input_Matrix, char **Output_Matrix, char typ, int mn)
{
	int x = 0;
	for (x =0 ; x < mn; x++)
	{
		int y = 0;
		for (y = 0;y < (int)strlen(Input_Matrix[x]);y++)
		{
			/*To traverse every string in the string matrix */
			if ( (typ == UPPER) || (typ == UPPER_B) )
			{
				/*converts the matrix of strings  str-matrix into upper case */
				if (Input_Matrix[x][y] > 0)
				{
					if (isalpha(Input_Matrix[x][y])) Output_Matrix[x][y] = (char)toupper(Input_Matrix[x][y]);
					else Output_Matrix[x][y] = (char)Input_Matrix[x][y];
				}
				else Output_Matrix[x][y] = (char)0;
			}
			else if ( (typ==LOW) || (typ==LOW_B) )
			{
				/*converts the matrix of strings  str-matrix  into lower case */
				if (Input_Matrix[x][y] > 0)
				{
					if (isalpha(Input_Matrix[x][y])) Output_Matrix[x][y] = (char)tolower(Input_Matrix[x][y]);
					else Output_Matrix[x][y] = (char)Input_Matrix[x][y];
				}
				else Output_Matrix[x][y] = (char)0;

			}
		}
        Output_Matrix[x][y] = 0;
	}
}
/*--------------------------------------------------------------------------*/
