
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
static int lengthMlist(void);
static int lengthTlist(void);
static int lengthOthers(char *fname);
/*----------------------------------------------------------------------------*/
int C2F(sci_length)(char *fname,unsigned long fname_len)
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

	
	case sci_tlist:
		lengthTlist();
	break;

	case sci_mlist:
		lengthMlist();
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
	#define RHSPOS 1
	int m = 0, n = 0; /* matrix size */
	int mn = 0; /* m*n */

	int il = 0; int ilrd = 0;
	int l1 = 0;

	int outIndex = 0 ;
	int x = 0;
	
	int lw = RHSPOS + Top - Rhs;
	
	l1 = *Lstk(lw);
	il = iadr(l1);

	if (*istk(il ) < 0) il = iadr(*istk(il + 1));

	/* get dimensions */
	m = getNumberOfLines(il); /* row */
	n = getNumberOfColumns(il); /* col */
	mn = m * n ;
	
	ilrd = il + 4;
	
	/* readjust stack before to call createvar */
	/* @TODO rewrite this without using intersci */
	C2F(intersci).ntypes[RHSPOS - 1] = '$';
	C2F(intersci).iwhere[RHSPOS - 1] = l1;
	C2F(intersci).lad[RHSPOS - 1] = l1;

	/* Create Variable on stack */
	CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &m,&n, &outIndex );
	for  ( x = 0; x < mn; x++ )
	{
		#define STK_POS_STRING ((integer *)&C2F(stack))
		/* put length of strings */
		/* beginning of string : STK_POS[ilrd + x ] */
		/* end of string : STK_POS[ilrd + x - 1] */
		stk(outIndex)[x] = (double) (STK_POS_STRING[ilrd + x ] - STK_POS_STRING[ilrd + x - 1]);
		#undef STK_POS_STRING
	}

	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();
	return 0;

	/* benchmark on Windows */
	/* C2D 6600 2.4 Ghz */
	/* fortran code : 135 microsecondes */
	/* optimized code stack2 : 146 microsecondes */
	/* code stack3 (commented) : 17629 microsecondes */
	/* Conclusion : GetRhsVar with strings is too slow ... */

	//char **Input_StringMatrix = NULL;
	//int Row_Num = 0,Col_Num = 0,mn = 0;

	///* When input character string or matrix of strings. */
	//GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num,&Col_Num,&Input_StringMatrix);
	//mn = Row_Num*Col_Num;  

	//if (Input_StringMatrix)
	//{
	//	int outIndex = 0 ;
	//	int x = 0;

	//	CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &Row_Num,&Col_Num, &outIndex );
	//	for  ( x = 0; x < mn; x++ )
	//	{
	//		stk(outIndex)[x] = (int)strlen(Input_StringMatrix[x]);
	//	}
	//	LhsVar(1) = Rhs+1 ;
	//	C2F(putlhsvar)();
	//	freeArrayOfString(Input_StringMatrix,Row_Num * Col_Num);
	//}
	//else
	//{
	//	freeArrayOfString(Input_StringMatrix,Row_Num * Col_Num);
	//	Scierror(999,_("%s: No more memory.\n"),fname);
	//}
	//return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthMatrix(void)
{
	int Row_Num = 0, Col_Num = 0;
	int Row_Out = 0, Col_Out = 0;
	int StackPos = 0;

	/*input matrix is a  matrix of double */
	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_Num,&Col_Num,&StackPos);

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
	Scierror(999, _("%s: Wrong type for input argument(s).\n"),fname);
	return 0;
}
/*--------------------------------------------------------------------------*/
static int lengthMlist(void)
{

	int Row_Num = 0,Col_Num = 0;
	int Row_Out = 0, Col_Out = 0;
	int StackPos = 0; 

	GetRhsVar(1,MATRIX_ORIENTED_TYPED_LIST_DATATYPE,&Row_Num,&Col_Num,&StackPos);

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
static int lengthTlist(void)
{

	int Row_Num = 0,Col_Num = 0;
	int Row_Out = 0, Col_Out = 0;
	int StackPos = 0; 

	GetRhsVar(1, TYPED_LIST_DATATYPE,&Row_Num,&Col_Num,&StackPos);

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
