/*------------------------------------------------------------------------*/
/* File: sci_grep.c                                                       */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu , Allan CORNET                                      */
/* desc : search position of a character string in an other string
          using regular express .                                         */
/*------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "gw_string.h"
#include "pcre.h"
#include "pcreposix.h"
#include "machine.h"
#include "stack-c.h"
#include "kmp.h"
#include "MALLOC.h" /* MALLOC */
#include "pcre_private.h"
#include "Scierror.h"
#include "localization.h"
/*------------------------------------------------------------------------*/
static int sci_grep_old(char *fname);
static int sci_grep_new(char *fname);
/*------------------------------------------------------------------------*/
int C2F(sci_grep) _PARAMS((char *fname,unsigned long fname_len))
{
    CheckRhs(2,3);
    CheckLhs(1,2);

	if (VarType(1) == sci_matrix)
	{
		int m1 = 0, n1 = 0;
		char **Str=NULL;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Str);

		if ((m1 == 0) && (n1 == 0))
		{
			int l = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;
		}
	}

	if (Rhs == 3)
	{
		if (VarType(3) == sci_strings)
		{
			char typ = 'd'; /*default */
			int m3 = 0,n3 = 0,l3 = 0;

			GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
			if ( m3*n3 != 0) typ = cstk(l3)[0];

			if (typ == 'r' )
			{
				sci_grep_new(fname);
			}
			else
			{
				Scierror(999,"3th argument invalid value.\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"3th argument invalid type.\n");
			return 0;
		}
	}
	else /* Rhs == 2 */
	{
		sci_grep_old(fname);
	}
    return 0;
}
/*-----------------------------------------------------------------------------------*/
static int sci_grep_old(char *fname)
{
	int x = 0, y = 0;

	int m1 = 0, n1 = 0;
	char **Strings_Input_One = NULL;
	int m1n1 = 0; /* m1 * n1 */

	int m2 = 0, n2 = 0;
	char **Strings_Input_Two = NULL;
	int m2n2 = 0; /* m2 * n2 */

	int *next = NULL;
	int *values = NULL;
	int *positions = NULL;

	int lengthMax = 0; /* MAX size of values and positions array */
	int currentLength = 0;

	int numRow   = 0;
	int outIndex = 0;

	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input_One);
	m1n1 = m1*n1;  
	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input_Two);
	m2n2 = m2*n2; 

	for (x = 0; x < m1n1 ;x++) 
	{
		lengthMax = lengthMax + (int)strlen(Strings_Input_One[x]);
	}

	next = (int *)MALLOC(sizeof(int)*(lengthMax+1));
	values = (int *)MALLOC(sizeof(int)*(lengthMax+1));
	positions = (int *)MALLOC(sizeof(int)*(lengthMax+1));

	if ( (next == NULL) || (values == NULL) || (positions == NULL) )
	{
		if (next) {FREE(next); next = NULL;}
		if (values) {FREE(values);values = NULL;}
		if (positions) {FREE(positions);positions = NULL;}
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	/* without using the regexp*/
	for (y = 0; y < m1n1; ++y)
	{
		for (x = 0; x < m2n2; ++x)
		{
			int w = 0;
			int pos = 0;
			if ( strlen(Strings_Input_Two[x]) == 0)
			{
				if (values) {FREE(values); values = NULL;}
				if (positions) {FREE(positions); positions = NULL;}
				if (next) {FREE(next); next=NULL;}

				Scierror(249,"%s : 2th argument must not be an empty string.\n",fname); 
				return 0;
			}

			getnext(Strings_Input_Two[x],next);

			/* Strings_Input_One is the input string matrix,
			 * Strings_Input_Two[x] is the substring to match
			 * pos is the start point*/
			w = kmp(Strings_Input_One[y],Strings_Input_Two[x],pos,next);  
			if (w != 0)
			{            
				if (currentLength < lengthMax) 
				{
					values[currentLength] = y+1;
					positions[currentLength] = x+1;
					currentLength++;
				}
			}     
		}
	}

	if (currentLength > lengthMax) currentLength = lengthMax;

	numRow   = 1;  /* Output values[]*/
	outIndex = 0;
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&currentLength,&outIndex);
	for ( x = 0 ; x < currentLength ; x++ )
	{
		stk(outIndex)[x] = (double)values[x] ;
	}
	LhsVar(1) = Rhs+1 ;

	if (Lhs == 2)
	{
		/* Output positions[]*/
		numRow   = 1;
		outIndex = 0;
		CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&currentLength,&outIndex);
		for ( x = 0 ; x < currentLength ; x++ )
		{
			stk(outIndex)[x] = (double)positions[x] ;
		}
		LhsVar(2) = Rhs+2;    
	}
	C2F(putlhsvar)();

	if (values) {FREE(values); values = NULL;}
	if (positions) {FREE(positions); positions = NULL;}
	if (next) {FREE(next); next = NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int sci_grep_new(char *fname)
{
	int x = 0, y = 0;

	int m1 = 0, n1 = 0;
	char **Strings_Input_One = NULL;
	int m1n1 = 0; /* m1 * n1 */

	int m2 = 0, n2 = 0;
	char **Strings_Input_Two = NULL;
	int m2n2 = 0; /* m2 * n2 */

	int *next = NULL;
	int *values = NULL;
	int *positions = NULL;

	int lengthMax = 0; /* MAX size of values and positions array */
	int currentLength = 0;

	int numRow   = 0;
	int outIndex = 0;

	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input_One);
	m1n1 = m1*n1;  
	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input_Two);
	m2n2 = m2*n2; 


	for (x = 0; x < m1n1 ;x++) 
	{
		lengthMax = lengthMax + (int)strlen(Strings_Input_One[x]);
	}

	next = (int *)MALLOC(sizeof(int)*(lengthMax+1));
	values = (int *)MALLOC(sizeof(int)*(lengthMax+1));
	positions = (int *)MALLOC(sizeof(int)*(lengthMax+1));

	if ( (next == NULL) || (values == NULL) || (positions == NULL) )
	{
		if (next) {FREE(next); next = NULL;}
		if (values) {FREE(values);values = NULL;}
		if (positions) {FREE(positions);positions = NULL;}
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}
	
	/*When we use the regexp;*/
	for ( y = 0; y < m1n1; ++y)
	{
		for ( x = 0; x < m2n2; ++x)
		{
			int Output_Start = 0;
			int Output_End = 0;
			int answer = pcre_private(Strings_Input_One[y],Strings_Input_Two[x],&Output_Start,&Output_End);

			if ( answer == 0 )
			{
				if (currentLength < lengthMax) 
				{
					values[currentLength] = y+1;
					positions[currentLength] = x+1;
					currentLength++;
				}
			}     
		}
	}

	if (currentLength > lengthMax) currentLength = lengthMax;

	numRow   = 1;  /* Output values[]*/
	outIndex = 0;
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&currentLength,&outIndex);
	for ( x = 0 ; x < currentLength ; x++ )
	{
		stk(outIndex)[x] = (double)values[x] ;
	}
	LhsVar(1) = Rhs+1 ;

	if (Lhs == 2)
	{
		/* Output positions[]*/
		numRow   = 1;
		outIndex = 0;
		CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&currentLength,&outIndex);
		for ( x = 0 ; x < currentLength ; x++ )
		{
			stk(outIndex)[x] = (double)positions[x] ;
		}
		LhsVar(2) = Rhs+2;    
	}
	C2F(putlhsvar)();

	if (values) {FREE(values); values = NULL;}
	if (positions) {FREE(positions); positions = NULL;}
	if (next) {FREE(next); next = NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
