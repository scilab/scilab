/*------------------------------------------------------------------------*/
/* File: sci_convstr1.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : converts the matrix of strings  str-matrix  into lower case (for*/  
/*          "l"  ;default value) or upper case (for  "u" ).               */
/*        [y]=convstr(str-matrix, ["flag"])
/*              str-matrix, y : matrices of strings                       */
/*              "flag"  : string ( "u"  for upper or  "l"  for lower 
/*                        (default value))                                */
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
/*-------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_strsplit1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str3;
  int *m4;
  int x,y,m1,n1,mn,i,n4,l4,u,v,w=0;
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
    GetRhsVar(2,"i",&m4,&n4,&l4);
    m4=istk(l4);
	u=0;
	v=0;
	w=0;
	Str3=(char**)MALLOC(sizeof(char*)*(m1*n1));
	for (i=0;i<20*20;i++)
	{
		Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str3[i],"");
	}
	for (x=0;x<mn;x++){
		if (x!=m4[v]) Str3[u][w++]=Str[0][x];
		else {
			u++;
			v++;
			w=0;
		}
        Str3[u][w]=0;
	}
	numRow   = u;  /*Output */
    numCol   = w ;
    CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3 ) ;
	LhsVar(1) = Rhs+1 ; 
    C2F(putlhsvar)();
    FREE(Str3);
  }
    return 0;
}

/*-----------------------------------------------------------------------------------*/
