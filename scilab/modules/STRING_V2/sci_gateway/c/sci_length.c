/*------------------------------------------------------------------------*/
/* File: sci_length.c                                                     */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : For usual or polynomial matrix  n  is the integer equal to 
   number of rows times number of columns of  M . (Also valid for  M  
   a boolean matrix) 

   For matrices made of character strings (and in particular for a 
   character string)  length  returns in  n  the length of entries of 
   the matrix of character strings  M . 

   The length of a list is the number of elements in the list 
   (also given by  size ). 

   length('123')  is  3 .  length([1,2;3,4])  is  4 .                     */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
/*-------------------------------------------------------------------------------------*/
/* get length */
static int lengthStrings(void);
static int lengthMatrix(void);
static int lengthList(void);
static int lengthOthers(void);
/*-------------------------------------------------------------------------------------*/
int C2F(sci_length) _PARAMS((char *fname,unsigned long fname_len))
{
  CheckRhs(1,1);
  CheckLhs(1,1);

  switch ( GetType(1)) 
  {
	case sci_strings : 
		lengthStrings();
	break;

	case sci_matrix :
		lengthMatrix();
	break;

	case sci_list:
		lengthList();
    break;

	default :
	   lengthOthers();
	break;
  }

  return 0;
}
/*-----------------------------------------------------------------------------------*/
static int lengthStrings(void)
{
	char **Input_StringMatrix = NULL;
	int Row_Num = 0,Col_Num = 0,mn = 0;
	double *Output_IntMatrix = NULL;

	/* When input character string or matrix of strings. */
	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num,&Col_Num,&Input_StringMatrix);
	mn = Row_Num*Col_Num;  

	Output_IntMatrix = (double*)MALLOC(sizeof(double)*(mn));
	if (Output_IntMatrix)
	{
		int x = 0;
		for (x = 0; x < mn; x++) Output_IntMatrix[x] = (int)strlen(Input_StringMatrix[x]);

		CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_Num,&Col_Num,&Output_IntMatrix);
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();

		if (Output_IntMatrix) { FREE(Output_IntMatrix); Output_IntMatrix=NULL;}
	}
	else
	{
		Scierror(999,"Error memory allocation.\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int lengthMatrix(void)
{
	int Row_Num = 0, Col_Num = 0;
	double *Output_IntMatrix = NULL;
	int StackPos = 0;

	/*When input vector of integer ascii codes  */
	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&Row_Num,&Col_Num,&StackPos);

	Output_IntMatrix=(double*)MALLOC(sizeof(double));
	if (Output_IntMatrix)
	{
		int numRow = 1;
		int numCol = 1;

		Output_IntMatrix[0]=Row_Num*Col_Num;

		CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&numCol,&Output_IntMatrix);
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();

		if (Output_IntMatrix) {FREE(Output_IntMatrix); Output_IntMatrix=NULL; }
	}
	else
	{
		Scierror(999,"Error memory allocation.\n");
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int lengthList(void)
{
	int Row_Num = 0,Col_Num = 0;
	double *Output_IntMatrix = NULL;
	int StackPos = 0; 

	GetRhsVar(1,LIST_DATATYPE,&Row_Num,&Col_Num,&StackPos);

	Output_IntMatrix = (double*)MALLOC(sizeof(double));
	if (Output_IntMatrix) 
	{
		int numRow = 1;
		int numCol = 1;

		Output_IntMatrix[0] = Row_Num*Col_Num;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&numCol,&Output_IntMatrix);
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();

		if (Output_IntMatrix) {FREE(Output_IntMatrix); Output_IntMatrix=NULL; }
	}
	else
	{
		Scierror(999,"Error memory allocation.\n");
		return 1;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int lengthOthers(void)
{
	/* unknow type */
	Scierror(999, "Incorrect Input type.\n");
	return 0;
}
/*-----------------------------------------------------------------------------------*/
