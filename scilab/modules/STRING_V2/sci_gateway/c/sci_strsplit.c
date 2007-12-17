/*------------------------------------------------------------------------*/
/* File: sci_strsplit.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "strsplit.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_strsplit) _PARAMS((char *fname,unsigned long fname_len))
{
	char **Input_StringMatrix=NULL;
	char **Output_StringMatrix=NULL;
	int *Input_IntMatrix=NULL;
	int Type_One = VarType(1);
	int Type_Two = VarType(2);
	int Row_Pointer = 0;
	int Row_Pointer_Two = 0;
	int Col_Pointer = 0;
	int Row_One = 0,Col_One = 0;
	int Row_Two = 0,mn = 0;
	int i = 0,Col_Two = 0,l4 = 0;
	int numRow = 0;
	int numCol = 0;
	


	CheckRhs(2,2);
	CheckLhs(1,1);
	
	switch (Type_One) 
	{
		case sci_matrix:
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&Input_StringMatrix);
		    if ((Row_One == 0) && (Col_One == 0))
			{
				int l = 0;
				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&l);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();
				return 0;
			}
		break;
		case sci_strings :
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_StringMatrix);
			mn = Row_One*Col_One;  
		break;
		default :
			Scierror(999,_("%s : Wrong type for first input argument: scalar or string matrix expected.\n"),fname);
			return 0;
		break;
	} 
    
    if (Type_Two == sci_matrix) 
	{
			GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&Row_Two,&Col_Two,&l4);
			Input_IntMatrix = istk(l4);
	}
	else
	{
			freeArrayOfString(Input_StringMatrix,mn);
			Scierror(999,_("%s : Wrong type for second input argument: scalar or string matrix expected.\n"),fname);
			return 0;
	}
	
	Output_StringMatrix = (char**)MALLOC(sizeof(char*)*(Col_Two+1));
	if (Output_StringMatrix == NULL)
	{
		freeArrayOfString(Input_StringMatrix,mn);
		Scierror(999,_("%s : Memory allocation error\n"),fname);
		return 0;
	}

	Output_StringMatrix[0] = (char*)MALLOC(sizeof(char*)*(Input_IntMatrix[0]));
	if (Output_StringMatrix[0] == NULL)
	{
		FREE(Output_StringMatrix);
		Output_StringMatrix = NULL;
		freeArrayOfString(Input_StringMatrix,mn);
		Scierror(999,_("%s : Memory allocation error\n"),fname);
		return 0;
	}

	for (i = 1; i < Col_Two ;i++)
	{
		Output_StringMatrix[i] = (char*)MALLOC(sizeof(char*)*(Input_IntMatrix[i]-Input_IntMatrix[i-1]));
		if (Output_StringMatrix[i] == NULL) /* Error of the Malloc */
		{
			freeArrayOfString(Output_StringMatrix,i);
			freeArrayOfString(Input_StringMatrix,mn);
			Scierror(999,_("%s : Memory allocation error\n"),fname);
			return 0;
		}
	}

	Output_StringMatrix[Col_Two] = (char*)MALLOC(sizeof(char*)*(strlen(Input_StringMatrix[0])-Input_IntMatrix[Col_Two-1]));
	if (Output_StringMatrix[Col_Two] == NULL)
	{
		freeArrayOfString(Input_StringMatrix,mn);
		Scierror(999,_("%s : Memory allocation error\n"),fname);
		return 0;
	}

	strsplit(Input_StringMatrix,Output_StringMatrix,&Row_Pointer,&Row_Pointer_Two,&Col_Pointer,Input_IntMatrix);

	freeArrayOfString(Input_StringMatrix,mn);

	/* put result on scilab stack */
	numRow =  Col_Two+1;
	numCol = 1 ;
	CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_StringMatrix );
	LhsVar(1) = Rhs + 1 ;
	C2F(putlhsvar)();

	/* free pointers */
	freeArrayOfString(Output_StringMatrix,Col_Two+1);
	return 0;
}
/*--------------------------------------------------------------------------*/
