/*------------------------------------------------------------------------*/
/* File: sci_str2code1.c                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : This function return scilab integer codes associated with a
          character string                                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "code2str.h"
#include "Scierror.h"
#include "localization.h"
/*-------------------------------------------------------------------------------------*/
int C2F(sci_str2code) _PARAMS((char *fname,unsigned long fname_len))
{
  CheckRhs(1,1);
  CheckLhs(1,1);

  if (VarType(1)==sci_strings)
		{
			char **Input_String = NULL;
			int m1 = 0,n1 = 0,i = 0;
			int *Output_Matrix = NULL;
			int nbOutput_Matrix = 0;
			int numRow   = 1 ;
			int outIndex = 0;
			int mn = 0;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_String);
			mn = m1*n1;
			if (strlen(Input_String[0]))  Output_Matrix=(int*)MALLOC(sizeof(int)*(strlen(Input_String[0])));
			else  Output_Matrix=(int*)MALLOC(sizeof(int));  

			/* Please check this */
			nbOutput_Matrix = str2code(Output_Matrix,Input_String);

			/* put on scilab stack */
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&nbOutput_Matrix,&numRow,&outIndex);    /*Output*/
			for ( i = 0 ; i < nbOutput_Matrix ; i++ )
			{
				stk(outIndex)[i] = (double)Output_Matrix[i] ;
			}
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();

			/* free pointers */
			if (Output_Matrix) {FREE(Output_Matrix); Output_Matrix=NULL; }
		}else{
			Scierror(999,_("Input argument must be a string.\n"));
		}
  return 0;
}
/*-------------------------------------------------------------------------------------*/
