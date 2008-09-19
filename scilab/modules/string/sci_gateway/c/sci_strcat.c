
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : concatenate character strings                                      */
/* Examples: strcat(string(1:10),',')                                     */ 
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h" 
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
#include "localization.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*-------------------------------------------------------------------------------------*/ 
#define STAR '*'
#define COL 'c'
#define ROW 'r'
#define ONE_CHAR 1
#define EMPTY_CHAR ""
/*-------------------------------------------------------------------------------------*/ 
static int sci_strcat_three_rhs(char *fname);
static int sci_strcat_two_rhs(char *fname);
static int sci_strcat_one_rhs(char *fname);
static int sci_strcat_rhs_one_is_a_matrix(char *fname);
static int sumlengthstring(int rhspos);
/*-------------------------------------------------------------------------------------*/
int C2F(sci_strcat)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,3);
	CheckLhs(1,1);

	switch (Rhs)
	{
		case 3:
		{
			sci_strcat_three_rhs(fname);
		}
		break;
		case 2:
		{
			sci_strcat_two_rhs(fname);
		}
		break;
		case 1:
		{
			sci_strcat_one_rhs(fname);
		}
		break;
		default:
		/* nothing */
		break;
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_three_rhs(char *fname) 
{ 
	char typ = 0;
	char **Input_String_One = NULL;
	char **Output_String = NULL;
	static char def_sep[] ="";
	char *Input_String_Two = def_sep;
	static int un=1;
	int Row_One = 0,Col_One = 0;
	int mn = 0;
	int i = 0,j = 0,k = 0;
	int Row_Two = 0,Col_Two = 0;
	int Row_Three = 0,Col_Three = 0;
	int l3 = 0,nchars = 0; 

	switch ( VarType(1)) 
	{
	case sci_strings :
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
		mn = Row_One*Col_One;  
		if (Rhs >= 2) 
		{ 
			/* second argument always a string and not a matrix of string */
			int l2 = 0;
			GetRhsVar(2,STRING_DATATYPE,&Row_Two,&Col_Two,&l2);
			Input_String_Two = cstk(l2);
		}
		if (Rhs >= 3) 
		{
			GetRhsVar(3,STRING_DATATYPE,&Row_Three,&Col_Three,&l3);
			if ( Row_Three*Col_Three != 0) typ = cstk(l3)[0];
			if (typ != COL && typ != ROW ) 
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: ''%s'' or ''%s'' expected.\n"),fname,3,"c","r");
				return 0;
			}
		}
		switch ( typ ) 
		{
		case STAR : 
			/* just return one string */ 
			for ( i = 0 ; i < mn ; i++ ) nchars += (int)strlen(Input_String_One[i]); 
			nchars += (mn-1)*(int)strlen(Input_String_Two);
			CreateVar(Rhs+1,STRING_DATATYPE,&un,&nchars,&l3);
			k=0;
			for ( i = 0 ; i < mn ; i++ ) 
			{
				for ( j =0 ; j < (int)strlen(Input_String_One[i]) ; j++ ) *cstk(l3+ k++) = Input_String_One[i][j]; 
				if ( i != mn-1) for ( j =0 ; j < (int)strlen(Input_String_Two) ; j++ ) *cstk(l3+ k++) = Input_String_Two[j];
			}
			freeArrayOfString(Input_String_One,mn);
			LhsVar(1) = Rhs+1  ;
			break;
		case COL: 
			/* return a column matrix */ 
			if ( (Output_String = (char**)MALLOC((Row_One+1)*sizeof(char *)))==NULL) 
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
			Output_String[Row_One]=NULL;
			for (i= 0 ; i < Row_One ; i++) 
			{
				/* length of row i */ 
				nchars = 0;
				for ( j = 0 ; j < Col_One ; j++ ) nchars += (int)strlen(Input_String_One[i+ Row_One*j]);
				nchars += (Col_One-1)*(int)strlen(Input_String_Two); 

				Output_String[i]=(char*)MALLOC((nchars+1)*sizeof(char));
				if ( Output_String[i] == NULL) 
				{
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				} 
				/* fill the string */
				strcpy(Output_String[i],Input_String_One[i]);

				if ( Output_String[i] == NULL) 
				{
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				} 

				for ( j = 1 ; j < Col_One ; j++ ) 
				{
					strcat(Output_String[i],Input_String_Two); 
					strcat(Output_String[i],Input_String_One[i+ Row_One*j]);
				}
				
			}
			
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &Row_One, &un, Output_String);
			freeArrayOfString(Input_String_One,mn);
			freeArrayOfString(Output_String,Row_One+1);
			LhsVar(1) = Rhs+1  ;
			
			break;
		case ROW: 
			/* return a row matrix */ 
			if ( (Output_String = MALLOC((Col_One+1)*sizeof(char *)))==NULL) 
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
			Output_String[Col_One]=NULL;
			for (j= 0 ; j < Col_One ; j++) 
			{
				/* length of col j */ 
				nchars = 0;
				for ( i = 0 ; i < Row_One ; i++ ) 
				{
					nchars += (int)strlen(Input_String_One[i+ Row_One*j]);
				}
				nchars += (Row_One-1)*(int)strlen(Input_String_Two); 

				Output_String[j] = strdup(Input_String_One[j*Row_One]);

				if ( Output_String[j] == NULL) 
				{
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				} 

				for ( i = 1 ; i < Row_One ; i++ ) 
				{
					strcat(Output_String[j],Input_String_Two); 
					strcat(Output_String[j],Input_String_One[i+ Row_One*j]);
				}
			}
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &un, &Col_One, Output_String);
			freeArrayOfString(Input_String_One,mn);
			freeArrayOfString(Output_String,Col_One+1);
			LhsVar(1) = Rhs+1  ;
			break;
		}
		break; 
	default : 
		OverLoad(1);
		break; 
	}

	C2F(putlhsvar)();
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_two_rhs(char *fname)
{
	int Type_One = VarType(1);
	int Type_Two = VarType(2);
	
	int Number_Inputs_Two = 0;
	char **Input_String_Two = NULL;
	
	if (Type_Two != sci_strings)
	{
		Scierror(246,_("%s: Wrong type for input argument #%d: Single string expected.\n"),fname); 
		return 0;
	}
	else /* sci_strings */
	{
		int Row_Two = 0,Col_Two = 0;
		
		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_String_Two);
		Number_Inputs_Two = Row_Two * Col_Two;
		/* check we have only a string as second parameter */
		if (Number_Inputs_Two != 1)
		{
			freeArrayOfString(Input_String_Two,Number_Inputs_Two);
			Scierror(36,"%s : Wrong type for input argument #%d: Single string expected.\n",fname,2);
			return 0;
		}
	}

	if ( (Type_One != sci_strings) && (Type_One != sci_matrix) )
	{
		freeArrayOfString(Input_String_Two,Number_Inputs_Two);
		Scierror(246,"%s: Wrong type for input argument #%d: Matrix of strings expected.\n",fname,1);
		return 0;
	}
	else
	{
		if (Type_One == sci_matrix)
		{
			freeArrayOfString(Input_String_Two,Number_Inputs_Two);
			sci_strcat_rhs_one_is_a_matrix(fname);
		}
		else /* sci_strings */
		{
			char **Input_String_One = NULL;
			int Row_One = 0,Col_One = 0;
			int Number_Inputs_One = 0;
			int length_output = 0;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
			Number_Inputs_One = Row_One * Col_One;

			if (Input_String_One)
			{
				if ( (Number_Inputs_One == 1) && (strcmp(Input_String_One[0],"")==0) )
				{
					/* With no input argument returns a zero length character string */
					int m1 = 0, n1 = 0, l1 = 0;

					CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();

					freeArrayOfString(Input_String_Two,Number_Inputs_Two);
					freeArrayOfString(Input_String_One,Number_Inputs_One);
					return 0;
				}
				else
				{
					if (Number_Inputs_One == 1)
					{
						length_output = (int)strlen(Input_String_One[0]);
					}
					else
					{
						int lengthInput_String_Two = (int)strlen(Input_String_Two[0]);
						length_output = sumlengthstring(1) + (int)(Number_Inputs_One)*lengthInput_String_Two - lengthInput_String_Two;
					}
				}
			}

			if (length_output > 0)
			{
				static int n1 = 0, m1 = 0;
				int outIndex = 0 ;
				char *Output_String = NULL;
				int i = 0;
				int Number_Inputs_OneLessOne = Number_Inputs_One - 1;

				m1 = length_output;
				n1 = 1;

				CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
				Output_String = cstk(outIndex);

				/* strcpy + strcat faster than sprintf */
				strcpy(Output_String,Input_String_One[0]);
				( 0 < Number_Inputs_OneLessOne )? strcat(Output_String,Input_String_Two[0]):0;

				for (i = 1; i < Number_Inputs_One; i++)
				{
					strcat(Output_String,Input_String_One[i]);
					( i < Number_Inputs_OneLessOne )? strcat(Output_String,Input_String_Two[0]):0;
				}

				LhsVar(1) = Rhs+1;
				C2F(putlhsvar)();
				freeArrayOfString(Input_String_One,Row_One*Col_One);
				freeArrayOfString(Input_String_Two,Number_Inputs_Two);
			}
			else
			{
				if (length_output == 0)
				{
					int one    = 1 ;
					int len   = (int)strlen(EMPTY_CHAR);
					int outIndex = 0 ;

					CreateVar(Rhs+1,STRING_DATATYPE,&len,&one,&outIndex);
					strcpy(cstk(outIndex),EMPTY_CHAR);
					LhsVar(1) = Rhs+1 ;
					C2F(putlhsvar)();

					freeArrayOfString(Input_String_Two,Number_Inputs_Two);
					freeArrayOfString(Input_String_One,Number_Inputs_One);
				}
				else
				{
					freeArrayOfString(Input_String_Two,Number_Inputs_Two);
					freeArrayOfString(Input_String_One,Number_Inputs_One);
					Scierror(999,_("%s : Wrong size for input argument(s).\n"),fname);
				}
			}
		}
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_one_rhs(char *fname)
{
	int Type_One = VarType(1);
	if ( (Type_One != sci_strings) && (Type_One != sci_matrix) )
	{
		Scierror(246,"%s: Wrong type for input argument #%d: Matrix of strings expected.\n",fname,1);
		return 0;
	}
	else
	{
		if (Type_One == sci_strings)
		{
			int lenstrcat =  sumlengthstring(1);

			if (lenstrcat >= 0)
			{
				char **Input_String_One = NULL;
				int m = 0, n = 0; /* matrix size */
				int mn = 0; /* m*n */
				int n1 = 1, m1 = lenstrcat;
				int outIndex = 0 ;

				char *Output_String = NULL;
				int i = 0;

				GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m,&n,&Input_String_One);
				mn = m * n;

				CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
				Output_String = cstk(outIndex);

				for (i = 0; i < mn; i++)
				{
					if ( i == 0)
					{
						strcpy(Output_String,Input_String_One[i]);
					}
					else
					{
						strcat(Output_String,Input_String_One[i]);
					}
				}
				LhsVar(1) = Rhs+1;
				C2F(putlhsvar)();
				if (Input_String_One) freeArrayOfString(Input_String_One,mn);
			}
			else
			{
				Scierror(999,_("%s: Wrong size for input argument(s).\n"),fname);
			}
		}
		else /* sci_matrix*/
		{
			sci_strcat_rhs_one_is_a_matrix(fname);
		}
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_rhs_one_is_a_matrix(char *fname)
{
	/* strcat([],'A') returns a empty string matrix */
	double *Input_String_One = NULL;
	int Row_One = 0,Col_One = 0;

	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&Input_String_One);

	/* check that we have [] */
	if ((Row_One == 0) && (Col_One == 0)) 
	{
		int one    = 1 ;
		int len   = (int)strlen(EMPTY_CHAR);
		int outIndex = 0 ;

		CreateVar(Rhs+1,STRING_DATATYPE,&len,&one,&outIndex);
		strcpy(cstk(outIndex),EMPTY_CHAR);
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty real matrix expected.\n"),fname,1);
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sumlengthstring(int rhspos)
{
	int sumlength = -1; /* error */
	if (VarType(1) == sci_strings)
	{
		int m = 0, n = 0; /* matrix size */
		int mn = 0; /* m*n */

		int il = 0; int ilrd = 0;
		int l1 = 0;
		
		int x = 0;
	
		int lw = rhspos + Top - Rhs;
		int lenstrcat = 0;
	
		l1 = *Lstk(lw);
		il = iadr(l1);

		if (*istk(il ) < 0) il = iadr(*istk(il + 1));

		/* get dimensions */
		m = getNumberOfLines(il); /* row */
		n = getNumberOfColumns(il); /* col */
		mn = m * n ;
	
		ilrd = il + 4;

		for  ( x = 0; x < mn; x++ )
		{
			lenstrcat +=  (int) (*istk(ilrd + x + 1) - *istk(ilrd + x));
		}
		sumlength = lenstrcat;
	}
	return sumlength;
}
/*-------------------------------------------------------------------------------------*/
