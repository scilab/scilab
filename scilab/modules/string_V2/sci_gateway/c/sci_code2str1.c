/*------------------------------------------------------------------------*/
/* File: sci_code2str1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : str=code2str(c)
          Returns character string associated with Scilab integer codes.
          str is such that c(i) is the Scilab integer code of part(str,i))*/
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h" 
#include "src_string.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_code2str1) _PARAMS((char *fname,unsigned long fname_len))
{                                                                  /* The table of the code */

  char **Str3;
  int m1,n1,i,l1=0;
  int *m4;                     /* Input code */
  int numRow   = 1        ;
  int outIndex = 0        ;
  int numCol;
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);
  switch ( GetType(1)) {
   case sci_matrix :                
	   GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
	   m4=istk(l1);                              /* The inputing of the code */
	   Str3=(char**)MALLOC(sizeof(char*)*(m1*n1));  /*Malloc as the input*/
	   for (i=0;i<m1*n1;i++)
	   {
		   Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		   strcpy(Str3[i],"");
	   }
	   code2str(Str3,m1,n1,m4);         /*the algorithm */
	   //for (x=0;x<m1*n1;x++)                     /* To look up in the table */
		  // if (m4[x]>=0) 
	   //      Str3[0][x]=m5[m4[x]];
		  // else Str3[0][x]=m6[abs(m4[x])];
	   //Str3[0][m1*n1]=0;
       numRow   = 1 ;     /*Output*/
       numCol   = m1*n1 ;
       outIndex = 0 ;
       CreateVar(Rhs+1,STRING_DATATYPE,&numRow,&numCol,&outIndex);
       strncpy(cstk(outIndex), &Str3[0][0] , m1*n1 ) ;
	   LhsVar(1) = Rhs+1 ;
	   C2F(putlhsvar)();
       FREE(Str3);                     /*Free the space */
  }
 return 0;
}

/*-----------------------------------------------------------------------------------*/
