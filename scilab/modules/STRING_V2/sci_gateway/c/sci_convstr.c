/*------------------------------------------------------------------------*/
/* File: sci_convstr.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : converts the matrix of strings  str-matrix  into lower case (for*/  
/*          "l"  ;default value) or upper case (for  "u" ).               */
/*        [y]=convstr(str-matrix, ["flag"])                               */
/*              str-matrix, y : matrices of strings                       */
/*              "flag"  : string ( "u"  for upper or  "l"  for lower      */
/*                        (default value))                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "convstr.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_convstr) _PARAMS((char *fname,unsigned long fname_len))
{
	char **Input_Matrix = NULL;
	char **Output_Matrix = NULL;
	char typ = LOW; /* define a constant in convstr.h */
	int numRow = 0;
	int numCol = 0;
	int Row_Num_One = 0;
	int Col_Num_One = 0;
	int mn = 0; /* Row_Num_One * Col_Num_One */
	int i = 0;

	int Type_One = 0;

	CheckRhs(1,2);
	CheckLhs(1,1);
	
	Type_One = VarType(1);

	if (Rhs == 2) 
	{
		int Type_Two = VarType(2);

		if (Type_Two == sci_strings) 
		{
			int Row_Num_Two = 0,Col_Num_Two = 0,Stack_Pos=0;

			GetRhsVar(2,STRING_DATATYPE,&Row_Num_Two,&Col_Num_Two,&Stack_Pos);
			if ( (Row_Num_Two*Col_Num_Two) == 1 )
			{
				/* To put "flag" into typ; whether "u" or "l" */
				typ = cstk(Stack_Pos)[0];
				if ( (typ != UPPER) && (typ != LOW) && (typ != UPPER_B) && (typ != LOW_B) ) 
				{
					Scierror(999,_("Second input argument must be 'u' (Upper) or 'l' (Lower).\n"));
					return 0;	
				}
			}
		}
		else
		{
			Scierror(999,_("%s : Second input argument has a wrong type, expecting scalar or string matrix.\n"),fname);
			return 0;
		}
	}

	switch (Type_One) 
	{
		case sci_strings :
		{
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num_One,&Col_Num_One,&Input_Matrix);     /* To input the string matrix */
			mn = Row_Num_One * Col_Num_One; 
		}
		break;
		case sci_matrix :
		{
			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_Num_One,&Col_Num_One,&Input_Matrix);
            if ( (Row_Num_One == 0) && (Col_Num_One == 0) )
			{
				int l =0;
				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_Num_One,&Col_Num_One,&l);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();
				return 0;
			}
			else 
			{
				Scierror(999,_("%s : First input argument has a wrong type, expecting scalar or string matrix.\n"),fname);
				return 0;
			}
		}
		break;
		default :
			Scierror(999,_("%s : First input argument has a wrong type, expecting scalar or string matrix.\n"),fname);
			return 0;
		break;
	} 
     
	Output_Matrix = (char**)MALLOC(sizeof(char*)*(mn));
	if (Output_Matrix == NULL)
	{
		Scierror(999,_("%s : Memory allocation error\n"),fname);
		return 0;
	}

	for (i = 0;i < mn;i++)
	{
		Output_Matrix[i] = (char*)MALLOC( sizeof(char*) * (strlen(Input_Matrix[i])+1) );
		if (Output_Matrix[i] == NULL)
		{
			int j = 0;
			for (j = 0;j < i;j++)
			{
				if (Output_Matrix[j]) { FREE(Output_Matrix[j]); Output_Matrix[j] = NULL;}
			}
			Scierror(999,("%s : Memory allocation error\n"),fname);
			return 0;
		}
	}
      
	/* convstr algorithm */
	convstr(Input_Matrix,Output_Matrix,typ,mn); 
	freeArrayOfString(Input_Matrix,mn);
      
	/* put on scilab stack */
	numRow   = Row_Num_One; 
	numCol   = Col_Num_One ;
	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_Matrix );
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	/* free pointers used */
	for (i = 0;i < mn;i++)
	{
		if (Output_Matrix[i]) { FREE(Output_Matrix[i]); Output_Matrix[i]=NULL;}
	}
	if (Output_Matrix) {FREE(Output_Matrix); Output_Matrix=NULL; }
	return 0;
}
/*--------------------------------------------------------------------------*/ 
