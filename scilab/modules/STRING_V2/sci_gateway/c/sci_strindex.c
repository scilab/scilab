/*------------------------------------------------------------------------*/
/* File: sci_strindex.c                                                  */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : search position of a character string in an other string
          using regular expression .                                         */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "machine.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre_private.h"
/*------------------------------------------------------------------------*/
#define CHAR_S 's'
#define CHAR_R 'r'
/*------------------------------------------------------------------------*/
int C2F(sci_strindex) _PARAMS((char *fname,unsigned long fname_len))
{
	char typ = CHAR_S;
    char **Str = NULL;
	char **Str2 = NULL;

	int i = 0; /* loop indice */

    int mn = 0; /* dimension parameter 1 m*n */

    int m2 = 0,n2 = 0;
	int mn2 = 0; /* m2*n2 */

    int outIndex = 0;
    int numRow = 1;

	int *values = NULL;
    int *position = NULL;

	int nbValues = 0;
    int nbposition = 0;
	
    CheckRhs(1,3);
    CheckLhs(1,2);

    if (VarType(1) == sci_matrix)
	{
		int m1 = 0;
		int n1 = 0;

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
	else
	{
		int m1 = 0;
		int n1 = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
		mn = m1*n1;  
	}

	
	if (mn != 1)
    {
		freeArrayOfString(Str,mn);
        Scierror(36, _("%s : Wrong size of the first input argument, expecting scalar.\n"));
        return 0;
    }

    GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
    mn2 = m2*n2;  

	if ( (int)strlen(Str[0]) == 0 ) 
	{
		values = (int *)MALLOC(sizeof(int));
		position = (int *)MALLOC(sizeof(int));
	}
	else
	{
		values = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
		position = (int *)MALLOC( sizeof(int) * ( strlen(Str[0])) );
	}

    if (Rhs >= 3)
    {
		int m3 = 0;
		int n3 = 0;
		int l3 = 0;

        GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);

        if ( m3*n3 != 0) typ = cstk(l3)[0];

        if (typ == CHAR_R )
        {
			int x = 0;
			int w = 0;

			int Output_Start = 0;
			int Output_End = 0;

			/*When we use the regexp;*/
            for (x = 0; x < mn2; ++x)
            {
                w = pcre_private(Str[0],Str2[x],&Output_Start,&Output_End);
                if ( w == 0)
                {         
                    values[nbValues++]=Output_Start+1;         /*adding the answer into the outputmatrix*/
                    position[nbposition++]=x+1;                /*The number according to the str2 matrix*/
                }     
            }
        }
    }
    else
    {   
		int x = 0;
		int pos = 0;
		/* When we do not use the regexp.*/
		for (x=0; x < mn2 ;++x)
        {
			int w = 0;
			int w1=0;
			
            if ( strlen(Str2[x]) == 0 )
            {
				if (values) {FREE(values); values = NULL;}
				if (position) {FREE(position); position = NULL;}
				Scierror(999, _("%s : Wrong size of the second input argument, expecting a not empty string.\n"));
				return 0;
            }

			if (Str2)
			{
				
				for (w=0; w<(int)strlen(Str[0]);w++)
				{
					pos=0;
					w1=w;
			        for ( ; Str[0][w1]==Str2[x][pos] && Str[0][w1]!='\0';w1++,pos++) ;
                    if (pos == (int)strlen(Str2[x])) 
						{
							values[nbValues++] = w1-(int)strlen(Str2[x])+1;
							position[nbposition++] = x+1;
							pos=0;
						}
				}
			}
        }
    }

	freeArrayOfString(Str,mn);
	freeArrayOfString(Str2,mn2);

    numRow   = 1;/* Output values[] */ 
    outIndex = 0;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
    for ( i = 0 ; i < nbValues ; i++ )
    {
		stk(outIndex)[i] = (double)values[i] ;
    }
    LhsVar(1) = Rhs+1 ;

	/* position[]*/
    numRow   = 1;
    outIndex = 0;
    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbposition,&outIndex);
    for ( i = 0 ; i < nbposition ; i++ )
    {
		stk(outIndex)[i] = (double)position[i] ;
    }
    LhsVar(2) = Rhs+2;    

    C2F(putlhsvar)();

    if (values) {FREE(values); values = NULL;}
    if (position) {FREE(position); position = NULL;}

    return 0;
}
/*-----------------------------------------------------------------------------------*/
