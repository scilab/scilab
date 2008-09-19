
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
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
static int isasciiStrings(char *fname);
static int isasciiMatrix(char *fname);
/*----------------------------------------------------------------------------*/
int C2F(sci_isascii)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_matrix)
	{
		isasciiMatrix(fname);
	}
	else if (GetType(1) == sci_strings)
	{
		isasciiStrings(fname);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix or matrix of strings expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int isasciiMatrix(char *fname)
{
	int Row_Num = 0,Col_Num = 0,Stack_Pos = 0;
	int outIndex = 0 ;
	int len = 0;
	/*When input vector of int ascii codes  */
	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&Row_Num,&Col_Num,&Stack_Pos);
	len = Row_Num * Col_Num ;

	if (len != 0)
	{
		int x = 0;
		int *Input_IntMatrix = NULL;
		int *Output_BooleanMatrix = NULL;

		Input_IntMatrix = istk(Stack_Pos);
		outIndex = 0 ;
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,&Row_Num,&Col_Num,&outIndex);
		Output_BooleanMatrix = istk(outIndex);
		for (x = 0; x < len; x++) 
		{
			if (isascii(Input_IntMatrix[x])) Output_BooleanMatrix[x] = (int)TRUE;
			else Output_BooleanMatrix[x] = (int)FALSE;
		}
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int isasciiStrings(char *fname)
{
	char **Input_StringMatrix = NULL;
	int x = 0,y = 0,Row_Num = 0,Col_Num = 0;

	int Length_Output_Matrix = 0;
	int *Output_BooleanMatrix = NULL;
	int nbOutput_IntMatrix = 0;
	int numRow = 1;

	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num,&Col_Num,&Input_StringMatrix);

	Length_Output_Matrix = 0;
	for (x = 0;x < Row_Num*Col_Num;x++) Length_Output_Matrix = Length_Output_Matrix + (int)strlen(Input_StringMatrix[x]);

	if (Length_Output_Matrix != 0) 
	{
		Output_BooleanMatrix = (int*)MALLOC(sizeof(int)*(Length_Output_Matrix));
	}
	else Output_BooleanMatrix = (int*)MALLOC(sizeof(int));

	if (Output_BooleanMatrix == NULL)
	{
		freeArrayOfString(Input_StringMatrix,Row_Num*Col_Num);
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}

	for (x = 0; x < Row_Num*Col_Num; x++) 
	{
		for (y = 0;y < (int)strlen(Input_StringMatrix[x]); y++)
		{
			if (isascii(Input_StringMatrix[x][y])) Output_BooleanMatrix[nbOutput_IntMatrix] = (int)TRUE;
			else Output_BooleanMatrix[nbOutput_IntMatrix] = (int)FALSE; 
			nbOutput_IntMatrix++;
		}
	}

	freeArrayOfString(Input_StringMatrix,Row_Num*Col_Num);
	CreateVarFromPtr(Rhs + 1,MATRIX_OF_BOOLEAN_DATATYPE,&numRow,&nbOutput_IntMatrix,&Output_BooleanMatrix);

	LhsVar(1) = Rhs + 1 ;
	C2F(putlhsvar)();
	if (Output_BooleanMatrix) { FREE(Output_BooleanMatrix); Output_BooleanMatrix = NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
