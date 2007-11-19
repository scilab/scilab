/*------------------------------------------------------------------------*/
/* File: sci_stripblanks.c                                                */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : strips leading and trailing blanks (and tabs) of strings  
          txt=stripblanks(txt[,tabs])   
          Parameters 
          txt : string or matrix of strings 
          tabs : if TRUE then tabs are also stripped (default value is FALSE)*/                                    
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h" 
#include "stripblanks.h"
/*-------------------------------------------------------------------------------------*/
int C2F(sci_stripblanks) _PARAMS((char *fname,unsigned long fname_len))
{
	char **Input_String_Matrix_One = NULL;
	char **Output_String_Matrix = NULL; 
	int numRow = 0;        /*@ The row number of the output string matrix*/
	int numCol = 0;        /*@ The col number of the output string matrix*/
	int m1 = 0,n1 = 0,mn = 0,i = 0,j = 0;
	BOOL bREMOVE_TAB = FALSE; /* DEFAULT no TAB */
    int Type_One = VarType(1);

	CheckRhs(1,2);
	CheckLhs(1,1);

	if (Rhs == 2)
	{
		int Type_Two = VarType(2);

		switch (Type_Two) 
		{
			case sci_boolean :
			{
				int m2 = 0, n2 = 0, l2 = 0;
				GetRhsVar(2,MATRIX_OF_BOOLEAN_DATATYPE,&m2,&n2,&l2);
				bREMOVE_TAB = (BOOL)*istk(l2);  
			}
			break;
			default :
			{
				Scierror(999,"%s : second argument has a wrong type, expecting a boolean.\n",fname);
				return 0;
			}
		} 
	}

	switch (Type_One) 
	{
		case sci_matrix :
			{
				/* case stripblanks([]) */
				GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Input_String_Matrix_One);
				if ( (m1 == 0) && (n1 == 0) )
				{
					int l = 0;
					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
					LhsVar(1) = Rhs+1 ;
					C2F(putlhsvar)();
					return 0;

				}
			}
			break;
		case sci_strings :
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_String_Matrix_One);
	        mn = m1*n1; 
		break;
		default :
			Scierror(999,"%s : first argument has a wrong type, expecting scalar or string matrix.\n",fname);
		return 0;
	} 

	if (mn > 0) Output_String_Matrix = (char**)MALLOC(sizeof(char*)*(mn));	
	else Output_String_Matrix = NULL;

	if (Output_String_Matrix == NULL)
	{
		Scierror(999,"%s : Error memory allocation.\n",fname);
		return 0;
	}

	for (i = 0; i < mn ; i++)  /*@ To malloc a space the same as input string*/
	{
		Output_String_Matrix[i] = (char*)MALLOC(sizeof(char)*(strlen(Input_String_Matrix_One[i])+1));
		if (Output_String_Matrix[i] == NULL)
		{
			for (j=0; j<i ; j++) 
			{
				if (Output_String_Matrix[j])  { FREE(Output_String_Matrix[j]) ; Output_String_Matrix[j] = NULL;}
			}

			Scierror(999,"%s : Error memory allocation.\n",fname);
			return 0;
		}
	}
	stripblanks(Input_String_Matrix_One,Output_String_Matrix,mn,bREMOVE_TAB); /*@ The stripblank function*/

	/* put result on scilab stack */
	numRow   = m1;
	numCol   = n1;
	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_String_Matrix); /*@ Output*/
	LhsVar(1) = Rhs+1 ; 
	C2F(putlhsvar)();

	/* free pointers */
	for (i=0;i < mn;i++)
	{
		if (Output_String_Matrix[i])  { FREE(Output_String_Matrix[i]) ; Output_String_Matrix[i]=NULL;}
	}
	if (Output_String_Matrix)  { FREE(Output_String_Matrix) ; Output_String_Matrix=NULL;}
	 
	return 0;
  }
/*-----------------------------------------------------------------------------------*/
