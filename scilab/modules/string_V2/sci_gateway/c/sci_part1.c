/*------------------------------------------------------------------------*/
/* File: part1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : Let  s[k]  stands for the  k  character of string  s  ( or the 
          white space character if  k >length(s) ).
          part  returns  c , a matrix of character strings, such that  
		  c(i,j)  is the string  "s[v(1)]...s[v(n)]"  (  s=mp(i,j)  ). 
                                                                          */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h" 
int numRow;
int numCol;
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
int C2F(sci_part1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str3;
  int x,y,m1,n1,mn,m2,n2,mn3,i,l2=0;
  unsigned *m;
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
	GetRhsVar(2,"i",&m2,&n2,&l2);
    m=istk(l2); 
	Str3=(char**)MALLOC(sizeof(char*)*(m1*n1));
	for (i=0;i<m1*n1;i++)
	{
		Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str3[i],"");
	}
	for (x=0;x<n1;x++){
        mn3=0;
		for (y=0;y<n2;y++)
			 if (m[y]-1<=strlen(Str[x])) Str3[x][mn3++]=Str[x][m[y]-1]		;
	}
	numRow   = m1 ;                          /*Output  */
    numCol   = n1 ;
    CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3 ) ;
	LhsVar(1) = Rhs+1 ; 
    C2F(putlhsvar)();
	FREE(Str3);
  }
    return 0;
}

/*-----------------------------------------------------------------------------------*/
