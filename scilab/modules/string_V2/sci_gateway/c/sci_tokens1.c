/*------------------------------------------------------------------------*/
/* File: sci_tokens1.c                                                    */
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
#include "returnProperty.h"
#include "machine.h"
#include "MALLOC.h" 
#include "src_string.h"
#define MAXSIZE 20
/*-----------------------------------------------------------------------------------*/
int C2F(sci_tokens1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str2,**Str3;
  int m1,n1,m2,n2,mn2,mn,i=0;
  int u=0;
  int w=0;
  int numRow;
  int numCol;
  Rhs = Max(0, Rhs);
  CheckRhs(1,2);
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
	if (Rhs == 2) 
	{
		GetRhsVar(2,"S",&m2,&n2,&Str2);
		mn2=m2*n2;
	}
	else 
	{ 
		Str2=(char**)MALLOC(sizeof(char*)*(MAXSIZE*MAXSIZE));
		Str2[0]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str2[0],"");
		Str2[0][0]=32;              /*The ascii of 32 is a ' ' */
		m2=1;
		n2=1;
		mn2=1;
	}
	Str3=(char**)MALLOC(sizeof(char*)*(MAXSIZE*MAXSIZE));
	for (i=0;i<MAXSIZE*MAXSIZE;i++)         /*Malloc*/
	{
		Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str3[i],"");
	}
	tokens(Str,Str2,Str3,&u,&w,mn,mn2);   /*The main function of tokens*/
	numRow   = u+1;  /*Output */
    numCol   = 1 ;
    CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3 ) ;
	LhsVar(1) = Rhs+1 ; 
    C2F(putlhsvar)();
    FREE(Str3);
    return 0;
}
/*-----------------------------------------------------------------------------------*/
