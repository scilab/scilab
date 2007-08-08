/*------------------------------------------------------------------------*/
/* File: sci_ascii1.c                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : This function convert Scilab string to a vector of ascii code   */
/*        or vector of ascii code to Scilab strings.                      */
/*        If  txt  is a matrix of string,  ascii(txt)  is equivalent to   */  
/*	      ascii(strcat(txt))                                              */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h" 
int numRow=1;
int numCol;
/*-------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
int C2F(sci_ascii1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str3;
  int x,y,m1,n1,mn,i,l1=0;
  int *m4;
  int values[100];
  int nbValues=0;
  int outIndex = 0        ;
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);
  switch ( GetType(1)) {
  case sci_strings :                  /* When input character string or matrix of strings. */
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
		mn = m1*n1;  
		for (x=0;x<mn;x++)
			for (y=0;y<strlen(Str[x]);y++)
				values[nbValues++]=Str[x][y];
		CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
		for ( i = 0 ; i < nbValues ; i++ )
		{
			stk(outIndex)[i] = (double)values[i] ;
		}
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
		return 0;
   case sci_matrix :                /*When input vector of integer ascii codes  */
	   GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
	   m4=istk(l1);
	   Str3=(char**)MALLOC(sizeof(char*)*(m1*n1));
	   for (i=0;i<m1*n1;i++)
	   {
		   Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		   strcpy(Str3[i],"");
	   }
	   for (x=0;x<m1*n1;x++)
	        Str3[0][x]=m4[x];
	   Str3[0][m1*n1]=0;
       numRow   = 1 ;
       numCol   = m1*n1 ;
       outIndex = 0 ;
       CreateVar(Rhs+1,STRING_DATATYPE,&numRow,&numCol,&outIndex);
       strncpy(cstk(outIndex), &Str3[0][0] , m1*n1 ) ;
	   LhsVar(1) = Rhs+1 ;
	   C2F(putlhsvar)();
       FREE(Str3);
	   return 0;
  }
  return 0;
}

/*-----------------------------------------------------------------------------------*/
