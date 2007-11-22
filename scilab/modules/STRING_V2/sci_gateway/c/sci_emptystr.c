/*------------------------------------------------------------------------*/
/* File: sci_emptystr1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : zero length string                                              */
/*        With no input argument returns a zero length character string   */
/*        With a matrix for input argument returns a zero length character*/
/*              strings matrix of the same size                           */
/*        With two integer arguments returns a mxn zero length character  */
/*                strings matrix                                          */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "returnProperty.h"
#include "machine.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "CreateEmptystr.h"
/*-----------------------------------------------------------------------------------*/
static int sci_emptystr_no_rhs(char *fname);
static int sci_emptystr_one_rhs(char *fname);
static int sci_emptystr_two_rhs(char *fname);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_emptystr) _PARAMS((char *fname,unsigned long fname_len))
{
   Rhs = Max(0, Rhs);
   CheckRhs(0,2);
   CheckLhs(1,1);
   if (Rhs < 1)
   {
	   sci_emptystr_no_rhs(fname);
   }
   else if (Rhs == 1)
   {
	   sci_emptystr_one_rhs(fname);
   }
   else if (Rhs == 2)
   {       
	   sci_emptystr_two_rhs(fname);
   }
   return 0;
}
/*-----------------------------------------------------------------------------------*/       
static int sci_emptystr_no_rhs(char *fname)
{
	/* With no input argument returns a zero length character string */
	int m1 = 0, n1 = 0, l1 = 0;
	
	CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/       
static int sci_emptystr_one_rhs(char *fname)
{
	char **OutputStrings = NULL;
	int m1 = 0,n1 = 0, l1 = 0;
	char **StringsFromStack = NULL;
	int i = 0;
	SciIntMat Im;

	/*With a matrix for input argument returns a zero length character strings matrix of the same size */
	int Type = VarType(1);
    int l=0;
	int Row_One=0; 
	int Col_One=0;
	char **Input_StringMatrix_One=NULL;
    if (VarType(1) == sci_matrix) 
	{
          GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&Input_StringMatrix_One);
	      if ((Row_One == 0) && (Col_One == 0))
			{
				l = 0;
				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&l);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();
				return 0;

			}
	}
	switch (Type) 
	{
		case sci_strings : 
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&StringsFromStack);
		break;
		case sci_matrix :
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		break;
		case sci_ints:
			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&Im);
		break;
		default :
			Scierror(999,"%s : first argument has a wrong type, expecting scalar or string matrix.\r\n",fname);
		return 0;
	} 
    
	OutputStrings=CreateEmptystr(m1,n1);

	if (OutputStrings == NULL)
	{
		Scierror(999,"%s : Error memory allocation.\r\n",fname);
		return 0;
	}

	/* m1 is the number of row ; n1 is the number of col*/
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1, &n1, OutputStrings);   
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	/* clean OutputStrings pointer */
	for (i=0;i<m1*n1;i++)
	{
		if (OutputStrings[i]) { FREE(OutputStrings[i]); OutputStrings[i]=NULL;}
	}
	if (OutputStrings) {FREE(OutputStrings); OutputStrings=NULL; }

	return 0;
}
/*-----------------------------------------------------------------------------------*/       
static int sci_emptystr_two_rhs(char *fname)
{
	int m1 = 0, n1 = 0, l1 = 0;
	int m2 = 0, n2 = 0, l2 = 0;
	int l=0;

	/*value_param_pos_1 is the number of row ; value_param_pos_2 is the number of col*/
	int value_param_pos_1 = 0;
	int value_param_pos_2 = 0;
	int matrixdimension = 0;

	char **OutputStrings = NULL;
	int Type_One = VarType(1);
	int Type_Two = VarType(2);



    
	/*With two integer arguments returns a m*n zero length character strings matrix */
	switch (Type_One) 
	{
		case sci_matrix:
			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
	        value_param_pos_1 = *istk(l1);
		break;
		default :
			Scierror(999,"%s : first argument must be intenger matrix.\r\n",fname);
		return 0;
	} 
    switch (Type_Two) 
	{
		case sci_matrix:
			GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
	        value_param_pos_2 = *istk(l2);
		break;
		default :
			Scierror(999,"%s : second argument must be intenger matrix.\r\n",fname);
		return 0;
	} 
	
	matrixdimension = value_param_pos_1 * value_param_pos_2;
    if (value_param_pos_1== 0 && value_param_pos_2==0)
	{
		l = 0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&value_param_pos_1,&value_param_pos_2,&l);
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
		return 0;
	}

	
	if (matrixdimension != 0)
	{
		int i = 0;
		OutputStrings = (char**)MALLOC(sizeof(char*)*(matrixdimension));
		if (OutputStrings == NULL)
		{
			Scierror(999,"%s : Error memory allocation.\r\n",fname);
			return 0;
		}

		for (i=0;i < matrixdimension;i++)              
		{
			OutputStrings[i] = (char*)MALLOC(sizeof(char*)*(strlen(EMPTY_STRING)+1));
			if (OutputStrings[i]) strcpy(OutputStrings[i],EMPTY_STRING);
		}

		CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE,&value_param_pos_1, &value_param_pos_2, OutputStrings);
		LhsVar(1) = Rhs + 1;
		C2F(putlhsvar)();

		/* free OutputStrings */
		for (i=0;i<matrixdimension;i++)
		{
			if (OutputStrings[i]) { FREE(OutputStrings[i]); OutputStrings[i] = NULL;}
		}
		if (OutputStrings) {FREE(OutputStrings); OutputStrings = NULL; }

		return 0;
	}
	else
	{
		OutputStrings = (char**)MALLOC(sizeof(char*)*(1));
		sci_emptystr_no_rhs(fname);
	}   
	return 0;
}
/*-----------------------------------------------------------------------------------*/       
