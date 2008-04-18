
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

/* desc : returns the tokens of a character string.  
          T=tokens(str [,delimiter])   
          str : a character string. The string where to search the tokens.
          delimiter : (optional) a character or a vector of characters.
                      The tokens delimeters.
          T : column vector of found tokens
                                                                          */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "tokens.h"
#include "localization.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define BLANK_CODE 32
#define EMPTY_STRING ""
/*--------------------------------------------------------------------------*/
int C2F(sci_tokens)(char *fname,unsigned long fname_len)
{
	char **Output_String = NULL;

	int Row_One = 0,Col_One = 0; 
	int mn_One = 0; /* Row_One * Col_One */
	char **Input_MatrixOne = NULL;

	int Row_Two = 0,Col_Two = 0;
	int mn_Two = 0; /* Row_Two * Col_Two */
	char **Input_MatrixTwo = NULL;
	
	int Row_Pointer = 0;
	int Col_Pointer = 0;
	int numRow = 0;
	int numCol = 0;

	CheckRhs(1,2);
	CheckLhs(1,1);
	
	if ( VarType(1) == sci_strings)
	{
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_MatrixOne);
		mn_One = Row_One * Col_One; 
	}
	else
	{
		Scierror(999,_("%s: Wrong type for first input argument: Scalar or matrix of strings expected.\n"),fname);
		return 0;
	}
	 
    if (Rhs == 2)
    {
		if ( VarType(2) == sci_strings )
		{
			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_MatrixTwo); 
			mn_Two = Row_Two * Col_Two;
		}
		else
		{
			freeArrayOfString(Input_MatrixOne,mn_One);
			Scierror(999,_("%s: Wrong type for first input argument: Scalar or matrix of strings expected.\n"),fname);
			return 0;
		}
    }
    else
    {
        Input_MatrixTwo = (char**)MALLOC(sizeof(char*));
		if (Input_MatrixTwo == NULL)
		{
			freeArrayOfString(Input_MatrixOne,mn_One);
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}

		Input_MatrixTwo[0] = strdup(EMPTY_STRING);

		if (Input_MatrixTwo[0] == NULL)
		{
			FREE(Input_MatrixTwo);
			Input_MatrixTwo = NULL;
			Scierror(999,_("%s: No more memory.\n"),fname);
			return 0;
		}
        Input_MatrixTwo[0][0] = BLANK_CODE;

        Row_Two = 1;
        Col_Two = 1;
        mn_Two = 1;
    }
    if (strlen(Input_MatrixOne[0]) == 0) 
	{
		Output_String = (char**)MALLOC( sizeof(char*));
	}
	else 
	{
		Output_String = (char**)MALLOC( sizeof(char*)*((int)strlen(Input_MatrixOne[0])));
	}

	if (Output_String == NULL)
	{
	    if (Input_MatrixTwo[0]) {FREE(Input_MatrixTwo[0]); Input_MatrixTwo[0]=NULL; }
        if (Input_MatrixTwo) {FREE(Input_MatrixTwo); Input_MatrixTwo=NULL; }

		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}

    if (strlen(Input_MatrixOne[0])==0) 
	{
		Output_String[0] = (char*)MALLOC( sizeof(char)*(1));
	}
	else 
	{
		int i = 0;
		for ( i = 0 ;i < (int)strlen(Input_MatrixOne[0]); i++)
		{
			Output_String[i] = (char*)MALLOC(sizeof(char*)*((int)strlen(Input_MatrixOne[0])));
			if (Output_String[i] == NULL)
			{
				freeArrayOfString(Output_String,i);
				freeArrayOfString(Input_MatrixTwo,1);
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}
	}

	tokens(Input_MatrixOne,Input_MatrixTwo,Output_String,&Row_Pointer,&Col_Pointer,mn_One,mn_Two);
	

	/* put result on stack */    
    numRow = Row_Pointer + 1;  /*Output */
    numCol = 1 ;
    if ( (numRow == 1) && ((int)strlen(Output_String[0]) == 0) ) 
	{
		int l = 0;
		numCol = 0;
		numRow = 0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&numCol,&l);
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
		return 0;
	}

	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_String );
    LhsVar(1) = Rhs+1 ;
    C2F(putlhsvar)();

	/* free pointers */
	if (Output_String)
	{
		if (Input_MatrixOne)
		{
			int i = 0;
			for (i = 0;i < (int)strlen(Input_MatrixOne[0]);i++)
			{
				if (Output_String[i]) { FREE(Output_String[i]); Output_String[i]=NULL;}
			}
			FREE(Output_String);
			Output_String=NULL; 
		}
	}

	if (Rhs==2)
	{
		freeArrayOfString(Input_MatrixTwo,mn_Two);
	}
	
	freeArrayOfString(Input_MatrixOne,mn_One);
    return 0;
}
/*--------------------------------------------------------------------------*/
