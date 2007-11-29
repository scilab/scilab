/*------------------------------------------------------------------------*/
/* File: sci_strindex.c                                                  */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : search position of a character string in an other string
          using regular express .                                         */
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
#include "kmp.h"
/*------------------------------------------------------------------------*/
int C2F(sci_strindex) _PARAMS((char *fname,unsigned long fname_len))
{
    
    int *next= NULL;   /* This const is just for testing. I will make it better soon*/
	char typ ='\0';
    char **Str=NULL;
	char **Str2=NULL;
    int x = 0,m1 = 0,n1 = 0,mn = 0,mn2 = 0,i = 0,m2 = 0,n2 = 0,m3 = 0,n3 = 0,l3=0;
    unsigned int x1 = 0;
    int outIndex = 0 ;
    char *lbuf=NULL;
    const size_t nmatch = 10;
    int  z = 0;
    int *values = NULL;
    int nbValues = 0;
    int *position = NULL;
    int nbposition = 0;
    int numRow = 1 ;
    int numCol = 1 ;
    int w = 0;
    int pos = 0;
	int Output_Start;
    int Output_End;
	int answer;
	int l;

    CheckRhs(1,3);
    CheckLhs(1,2);

    if (VarType(1) == sci_matrix)
	{
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Str);
		if ((m1 == 0) && (n1 == 0))
		{
			l = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;
		}
	}
	else
	{
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
		mn = m1*n1;  
	}

	
	if (mn != 1)
    {
        Scierror(36, "first argument is incorrect \n");
        return 1;
    }
    GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
    mn2 = m2*n2;  
	if ((int)strlen(Str[0])==0) 
	{
		next=(int *)MALLOC(1);
		values=(int *)MALLOC(1);
		position=(int *)MALLOC(1);
	}
	else
	{
		next=(int *)MALLOC(sizeof(int)*(strlen(Str[0])*strlen(Str[0])));
		values=(int *)MALLOC(sizeof(int)*(strlen(Str[0])*strlen(Str[0])));
		position=(int *)MALLOC(sizeof(int)*(strlen(Str[0])*strlen(Str[0])));
	}

    lbuf = *Str;
    if (Rhs >= 3)
    {
        GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
        if ( m3*n3 != 0) typ = cstk(l3)[0];

        if (typ == 'r' )
        {
			/*When we use the regexp;*/
            for (x = 0; x < mn2; ++x)
            {
                answer=pcre_private(Str[0],Str2[x],&Output_Start,&Output_End);
                if (answer==0)
                {         
                    values[nbValues++]=Output_Start+1;         /*adding the answer into the outputmatrix*/
                    position[nbposition++]=x+1;                /*The number according to the str2 matrix*/
                }     
            }
        }
    }
    else
    {                                    /* When we do not use the regexp.*/
        for (x=0;x<mn2;++x)
        {
            if (strlen(Str2[x])==0)
            {
               Scierror(999, "2th argument must not be an empty string.\n");
               return 0;
            }
            do
            {
                getnext(Str2[x],next);
                w = kmp(*Str,Str2[x],pos,next);      /*Str is the input string matrix, Str2[x] is the substring to match; pos is the start point*/
                if (w !=0)
                {            
                    values[nbValues++]=w;
                    position[nbposition++]=x+1;
                }
                pos=w;
            }
            while(w!=0);          /* w is the answer of the kmp algorithem*/
        }
    }

    numRow   = 1        ;/* Output values[] and position[]*/
    outIndex = 0        ;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
    for ( i = 0 ; i < nbValues ; i++ )
    {
          stk(outIndex)[i] = (double)values[i] ;
    }
    LhsVar(1) = Rhs+1 ;
    numRow   = 1        ;
    outIndex = 0        ;
    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbposition,&outIndex);
    for ( i = 0 ; i < nbposition ; i++ )
    {
        stk(outIndex)[i] = (double)position[i] ;
    }
    LhsVar(2) = Rhs+2;    
    C2F(putlhsvar)();
    if (next) {FREE(next); next=NULL;}
    if (values) {FREE(values); values=NULL;}
    if (position) {FREE(position); position=NULL;}

    return 0;
}
/*-----------------------------------------------------------------------------------*/
