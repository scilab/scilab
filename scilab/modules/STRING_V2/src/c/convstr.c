/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "convstr.h"
/*-----------------------------------------------------------------------------------*/
void convstr(char **Input_Matrix, char **Output_Matrix, char typ, int mn)
{
	int x = 0;
	for (x =0 ; x < mn; x++)
	{
		int y = 0;
		for (y=0;y < (int)strlen(Input_Matrix[x]);y++)
		{
			/*To traverse every string in the string matrix */
			if ( (typ == UPPER) || (typ == UPPER_B) )
			{
				/*converts the matrix of strings  str-matrix  into upper case */
				if    ( (Input_Matrix[x][y] >= Letter_a) && (Input_Matrix[x][y] <= Letter_z) )  
				{
					/* If it is the Lowercase letters */
            		Output_Matrix[x][y] = Input_Matrix[x][y] - Conversion_Letter		;
				}
				else Output_Matrix[x][y] = Input_Matrix[x][y];
			}
			else if ( (typ==LOW) || (typ==LOW_B) )
			{
				/*converts the matrix of strings  str-matrix  into lower case */
				if ( (Input_Matrix[x][y] >= Letter_A) && (Input_Matrix[x][y] <= Letter_Z) )
				{
					/* if it is the uppercase letters*/
					Output_Matrix[x][y] = Input_Matrix[x][y]+Conversion_Letter		;
				}
                else Output_Matrix[x][y] = Input_Matrix[x][y];
			}
		}
        Output_Matrix[x][y] = 0;
	}
}
/*-----------------------------------------------------------------------------------*/
