/*------------------------------------------------------------------------*/
/* File: sci_strcat1.c                                                    */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : catenate character strings                                      */
/* Examples: strcat1(string(1:10),',')                                    */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "machine.h"
#include "src_string.h"
/*-------------------------------------------------------------------------------------*/
int C2F(sci_strcat1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str2; /**< the two input arguments */
  int m1,n1,m2,n2,mn,mn2,numRow,numCol;
  Rhs = Max(0, Rhs);
  CheckRhs(1,2);
  GetRhsVar(1,"S",&m1,&n1,&Str);
  mn = m1*n1;    /*@ the first argument*/
  GetRhsVar(2,"S",&m2,&n2,&Str2);
  mn2 = m2*n2;  /*@ the second argument*/
  strcat(Str[0] ,Str2[0]);
  scistrcat(Str,Str2,mn);
  numRow   = 1 ;
  numCol   = 1 ;
  CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str ) ;
  LhsVar(1) = Rhs+1 ; 
  C2F(putlhsvar)();
  return 0;
}
