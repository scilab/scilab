/*------------------------------------------------------------------------*/
/* File: sci_grep.c                                                       */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : search position of a character string in an other string
          using regular express .                                         */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include <ctype.h>
#include "returnProperty.h"
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "kmp.h"
#include "MALLOC.h" /* MALLOC */
#include "pcre_private.cpp"
/*------------------------------------------------------------------------*/
int *next=NULL;
void getnext(char T[],int *next);
int kmp(char S[],char T[],int pos,int *);
int pcre_private(char *INPUT_LINE,char *INPUT_PAT,int *Output_Start,int *Output_End);
int C2F(sci_grep) _PARAMS((char *fname,unsigned long fname_len))
{
    char typ ;
    char **Str=NULL;
	char **Str2=NULL;
    int y,x,m1=0,n1=0,mn,mn2,i,m2=0,n2=0,m3,n3,l3=0;
    int outIndex = 0 ;
    int *values=NULL;
    int nbValues=0;
    int *position=NULL;
    int nbposition=0;
    int numRow   = 1 ;
    int w,l;
    int pos=0;
   int answer;
   int Output_Start;
   int Output_End;
	int typeInput = 0;
	int length;
    CheckRhs(1,3);
    CheckLhs(1,2);

	typeInput = VarType(1);

	if (typeInput == sci_matrix)
	{
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Str);
		
		if ((m1==0) && (n1==0))
		{
			l = 0;
            CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;


		}
	}
	//else
	

    GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
    mn = m1*n1;  
    GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
    mn2 = m2*n2;  
	length=0;
    for (x=0; x<mn;x++) 
	{
		length=length+strlen(Str[x]);
	}
    
	if (length==0) 
	{
		next=(int *)MALLOC(1);
		values=(int *)MALLOC(1);
		position=(int *)MALLOC(1);
	}
	else 
	{
		next=(int *)MALLOC(sizeof(int)*(length));
		values=(int *)MALLOC(sizeof(int)*(length));
		position=(int *)MALLOC(sizeof(int)*(length));
	}

    if (Rhs >= 3)
    {
		GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
        if ( m3*n3 != 0)
            typ = cstk(l3)[0];
        if (typ == 'r' )
        {            /*When we use the regexp;*/
			
            for (y=0;y<mn;++y)
            {
                for (x=0;x<mn2;++x)
                {
                    answer=pcre_private(Str[y],Str2[x],&Output_Start,&Output_End);
					if (answer==0)
                    {
                        values[nbValues++]=y+1; /*adding the answer into the outputmatrix*/
                        position[nbposition++]=x+1;  /*The number according to the str2 matrix*/
                    }     
                }
            }
        }/* end of typ=='r'*/
    }/* end of Rhs>=3*/
    else
    {                            /* without using the regexp*/
        nbposition=0;
        for (y=0;y<mn;++y)
        {
            for (x=0;x<mn2;++x)
            {
                if (strlen(Str2[x])==0)
                {
                    Scierror(249,"%s:2th argument must not be an empty string\r\n",fname); 
					return 1;
                }
                getnext(Str2[x],next);
                w=kmp(Str[y],Str2[x],pos,next);    /*Str is the input string matrix, Str2[x] is the substring to match; pos is the start point*/
                if (w !=0)
                {            
                    values[nbValues++]=y+1;
                    position[nbposition++]=x+1;
                }     
            }
        }
    }
    numRow   = 1        ;  /* Output values[] and position[]*/
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
	if (values) {FREE(values); values=NULL;}
    if (position) {FREE(position); position=NULL;}
    if (next) {FREE(next); next=NULL;}
    return 0;
}

/*-----------------------------------------------------------------------------------*/
