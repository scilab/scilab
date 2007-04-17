/*------------------------------------------------------------------------*/
/* File: sci_emptystr1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : zero length string                                              */
/*        With no input argument returns a zero length character string   */
/*        With a matrix for input argument returns a zero length character*/ 
/*              strings matrix of the same size                           */
/*        With two integer arguments returns a mxn zero length character  */
/*                strings matrix                                          */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "returnProperty.h"
#include "machine.h"
#include "MALLOC.h" 
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_emptystr1) _PARAMS((char *fname,unsigned long fname_len))
{
   char **Str,**Str4;
   int m1,n1,mn,m2,n2,i,m4,n4,l1,l2=0;
   int pos=0;
   Rhs = Max(0, Rhs);
   if (Rhs < 1) {  /* With no input argument returns a zero length character string */ 
		int m1=0;
		int n1=0;
		int l1=0;
		CreateVar(1,"c",  &m1, &n1, &l1);
		LhsVar(1)=1;
		C2F(putlhsvar)();
	    return 0;
   }
   if (Rhs ==1) {    /*With a matrix for input argument returns a zero length character strings matrix of the same size */
		GetRhsVar(1,"S",&m1,&n1,&Str);
		mn = m1*n1;  
		l1=0;
		Str4=(char**)MALLOC(sizeof(char*)*(m1*n1));
		for (i=0;i<m1*n1;i++)
			{
				Str4[i]=(char*)MALLOC(sizeof(char*)*(1));
				strcpy(Str4[i],"");
			}
		CreateVarFromPtr(1,"S",&m1, &n1, Str4);
		LhsVar(1)=1;
   }
   if (Rhs == 2){  /*With two integer arguments returns a mxn zero length character strings matrix */
		GetRhsVar(1,"i",&m1,&n1,&l1);
		m4=*istk(l1);
		GetRhsVar(2,"i",&m2,&n2,&l2);
		n4=*istk(l2);
		if (m4*n4!=0) Str4=(char**)MALLOC(sizeof(char*)*(m4*n4));
		for (i=0;i<m4*n4;i++)
		{
			Str4[i]=(char*)MALLOC(sizeof(char*)*(1));
			strcpy(Str4[i],"");
		}
		CreateVarFromPtr(1,"S",&m4, &n4, Str4);
		LhsVar(1)=1;
   }
   C2F(putlhsvar)();
   FREE(Str4);
   return 0;
}

/*-----------------------------------------------------------------------------------*/
