/*------------------------------------------------------------------------*/
/* File: part.c                                                           */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : Let  s[k]  stands for the  k  character of Input_StringMatrixings
  ( or the  white space character if  k >length(s) ).
  part  returns  c , a matrix of character Input_StringMatrixings, such that  
  c(i,j)  is the Input_StringMatrixing  "s[v(1)]...s[v(n)]"  (  s=mp(i,j)  ).
                                                                          */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#define BLANK_CHAR ' '
/*--------------------------------------------------------------------------*/
int C2F(sci_part) _PARAMS((char *fname,unsigned long fname_len))
{
    int numRow = 0;
    int numCol = 0;
	char **Output_StringMatrix = NULL;
    int x = 0,RowCol = 0;

	int Row_One = 0,Col_One = 0;
	char **Input_StringMatrix = NULL;

	int Row_Two = 0,Col_Two = 0;
	int StackPosTwo = 0;
    int *SecondParamaterValue = NULL;

    CheckRhs(2,2);
    CheckLhs(1,1);

    GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_StringMatrix);
    RowCol = Row_One*Col_One;  
	
	GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&Row_Two,&Col_Two,&StackPosTwo);
	SecondParamaterValue = istk(StackPosTwo);
	if (SecondParamaterValue[0] == 0) 
	{
		Scierror(36,_(" 2th argument is incorrect here\n"));
		return 0;
	}
    
    Output_StringMatrix = (char**)MALLOC(sizeof(char*)*(Col_One));
	if (Output_StringMatrix == NULL)
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

    for (x = 0; x < RowCol; x++)
    {
		int Char_Position = 0;
		int y = 0;

        Output_StringMatrix[x]=(char*)MALLOC(sizeof(char*)*(Col_Two+1));

		if (Output_StringMatrix == NULL)
		{
			for(y=0; y <= x; y++) 
			{
				if (Output_StringMatrix[y]) { FREE(Output_StringMatrix[y]); Output_StringMatrix[y] = NULL; }
			}
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}
        
        for (y=0;y < Col_Two*Row_Two;y++)
        {
             if ( SecondParamaterValue[y] <= (int)strlen(Input_StringMatrix[x]) )
             {
                 Output_StringMatrix[x][Char_Position]=Input_StringMatrix[x][SecondParamaterValue[y]-1];
             }
             else
             {
                 Output_StringMatrix[x][Char_Position] = BLANK_CHAR;
             }
			 Char_Position++;
        }
    }

	/*Output */
    numRow   = Row_One ;
    numCol   = Col_One ;

    CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_StringMatrix );
    LhsVar(1) = Rhs+1 ;
    C2F(putlhsvar)();

	/* FREE memory */
    for (x = 0;x < RowCol;x++)
    {
       if (Output_StringMatrix[x]) { FREE(Output_StringMatrix[x]); Output_StringMatrix[x]=NULL;}
    }
    if (Output_StringMatrix) {FREE(Output_StringMatrix); Output_StringMatrix=NULL; }

    return 0;
}

/*--------------------------------------------------------------------------*/
