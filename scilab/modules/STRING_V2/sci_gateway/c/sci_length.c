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
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
/* get length */
static int lengthStrings(char *fname);
static int lengthMatrix(void);
static int lengthList(void);
static int lengthOthers(char *fname);
/*----------------------------------------------------------------------------*/
int C2F(sci_length) _PARAMS((char *fname,unsigned long fname_len))
{
  CheckRhs(1,1);
  CheckLhs(1,1);

  switch ( GetType(1)) 
  {
	case sci_strings : 
		lengthStrings(fname);
	break;

	case sci_matrix :
		lengthMatrix();
	break;

	case sci_list:
		lengthList();
    break;

	default :
	   lengthOthers(fname);
	break;
  }

  return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthStrings(char *fname)
{
	char **Input_StringMatrix = NULL;
	int Row_Num = 0,Col_Num = 0,mn = 0;

	/* When input character string or matrix of strings. */
	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num,&Col_Num,&Input_StringMatrix);
	mn = Row_Num*Col_Num;  

	if (Input_StringMatrix)
	{
		int outIndex = 0 ;
		int x = 0;

		CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &Row_Num,&Col_Num, &outIndex );
		for  ( x = 0; x < mn; x++ )
		{
			stk(outIndex)[x] = (int)strlen(Input_StringMatrix[x]);
		}
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
		freeArrayOfString(Input_StringMatrix,Row_Num * Col_Num);
	}
	else
	{
		freeArrayOfString(Input_StringMatrix,Row_Num * Col_Num);
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthMatrix(void)
{
	int Row_Num = 0, Col_Num = 0;
	int Row_Out = 0, Col_Out = 0;
	int StackPos = 0;

	/*When input vector of integer ascii codes  */
	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&Row_Num,&Col_Num,&StackPos);

	StackPos = 0;
	Row_Out = 1;
	Col_Out = 1;

	CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &Row_Out,&Col_Out, &StackPos );
	stk(StackPos)[0] = (double)(Row_Num*Col_Num);
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();
	
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthList(void)
{
	int Row_Num = 0,Col_Num = 0;
	int Row_Out = 0, Col_Out = 0;
	int StackPos = 0; 

	GetRhsVar(1,LIST_DATATYPE,&Row_Num,&Col_Num,&StackPos);

	StackPos = 0;
	Row_Out = 1;
	Col_Out = 1;

	CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &Row_Out,&Col_Out, &StackPos );
	stk(StackPos)[0] = (double)(Row_Num*Col_Num);
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthOthers(char *fname)
{
	/* unknow type */
	Scierror(999, _("%s : Not managed input type.\n"),fname);
	return 0;
}
/*--------------------------------------------------------------------------*/
