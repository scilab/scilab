/*------------------------------------------------------------------------*/
/* File: sci_tokens.c                                                    */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
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
#include <ctype.h>
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "tokens.h"
/*-----------------------------------------------------------------------------------*/
#define BLANK_CODE 32
#define EMPTY_STRING ""
/*-----------------------------------------------------------------------------------*/
int C2F(sci_tokens) _PARAMS((char *fname,unsigned long fname_len))
{
	char **Input_MatrixOne = NULL;
	char **Input_MatrixTwo = NULL;
	char **Output_String = NULL;
	int Row_One = 0,Col_One = 0,Row_Two = 0,Col_Two = 0,mn2 = 0,mn = 0,i = 0;
	int Row_Pointer = 0;
	int Col_Pointer = 0;
	int numRow = 0;
	int numCol = 0;
	int Type_One = VarType(1);
	int l;

	

	int Type_Two = VarType(2);

	CheckRhs(1,2);
	CheckLhs(1,1);
	
	switch (Type_One) 
	{
		case sci_strings :
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_MatrixOne);
			mn = Row_One * Col_One; 
		break;

		default :
			Scierror(999,_("%s : wrong input argument has a wrong type, expecting scalar or string matrix.\n"),fname);
		return 0;
	} 
	 
    if (Rhs == 2)
    {
			switch (Type_Two) 
			{
				case sci_strings :
					 GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_MatrixTwo); 
                     mn2 = Row_Two * Col_Two;
				break;
				default :
					Scierror(999,_("%s : first input argument has a wrong type, expecting scalar or string matrix.\n"),fname);
					return 0;
				break;
			} 
    }
    else
    {
        Input_MatrixTwo = (char**)MALLOC(sizeof(char*));
		if (Input_MatrixTwo == NULL)
		{
			Scierror(999,_("%s : Memory allocation error\n"),fname);
			return 0;
		}

        Input_MatrixTwo[0] = (char*)MALLOC(sizeof(char*)*((int)strlen(EMPTY_STRING)+1));

		if (Input_MatrixTwo[0] == NULL)
		{
			FREE(Input_MatrixTwo);
			Input_MatrixTwo = NULL;
			Scierror(999,_("%s : Memory allocation error\n"),fname);
			return 0;
		}

        strcpy(Input_MatrixTwo[0],EMPTY_STRING);
        Input_MatrixTwo[0][0] = BLANK_CODE;

        Row_Two = 1;
        Col_Two = 1;
        mn2 = 1;
    }
    if (strlen(Input_MatrixOne[0])==0) 
	{
		Output_String = (char**)MALLOC( sizeof(char*)*(1));
	}
	else 
	{
		Output_String = (char**)MALLOC( sizeof(char*)*((int)strlen(Input_MatrixOne[0])));
	}
	if (Output_String== NULL)
	{
	    if (Input_MatrixTwo[0]) {FREE(Input_MatrixTwo[0]); Input_MatrixTwo[0]=NULL; }
        if (Input_MatrixTwo) {FREE(Input_MatrixTwo); Input_MatrixTwo=NULL; }

		Scierror(999,_("%s : Memory allocation error\n"),fname);
		return 0;
	}

    if (strlen(Input_MatrixOne[0])==0) 
	{
		Output_String[0] = (char*)MALLOC( sizeof(char)*(1));
	}
	else 
	{
		for ( i = 0 ;i < (int)strlen(Input_MatrixOne[0]); i++)
		{
			Output_String[i] = (char*)MALLOC(sizeof(char*)*((int)strlen(Input_MatrixOne[0])));
			if (Output_String[i] == NULL)
			{
				if (Output_String) {FREE(Output_String); Output_String=NULL; }
				if (Input_MatrixTwo[0]) {FREE(Input_MatrixTwo[0]); Input_MatrixTwo[0]=NULL; }
				if (Input_MatrixTwo) {FREE(Input_MatrixTwo); Input_MatrixTwo=NULL; }

				Scierror(999,_("%s : Memory allocation error\n"),fname);
				return 0;
			}
		}
	}


	tokens(Input_MatrixOne,Input_MatrixTwo,Output_String,&Row_Pointer,&Col_Pointer,mn,mn2);

	/* put result on stack */    
    numRow = Row_Pointer + 1;  /*Output */
    numCol = 1 ;
    if (numRow == 1 && strlen(Output_String[0])==0) 
	{
            l = 0;
			numCol=0;
			numRow=0;
            CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&numCol,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;
	}
	

	CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_String );
    LhsVar(1) = Rhs+1 ;
    C2F(putlhsvar)();

	/* free pointers */

    for (i=0;i < (int)strlen(Input_MatrixOne[0]);i++)
    {
           if (Output_String[i]) { FREE(Output_String[i]); Output_String[i]=NULL;}
    }
    if (Output_String) {FREE(Output_String); Output_String=NULL; }
    if (Input_MatrixTwo[0]) {FREE(Input_MatrixTwo[0]); Input_MatrixTwo[0]=NULL; }
    if (Input_MatrixTwo) {FREE(Input_MatrixTwo); Input_MatrixTwo=NULL; }

    return 0;
}
/*-----------------------------------------------------------------------------------*/
