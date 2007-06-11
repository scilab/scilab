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
/*-------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_strcat1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str2; /**< the two input arguments */
  int x,m1,n1,m2,n2,mn,mn2,numRow,numCol;
  Rhs = Max(0, Rhs);
  CheckRhs(1,2);
  switch ( VarType(1)) {
  case 10 :
		GetRhsVar(1,"S",&m1,&n1,&Str);
		mn = m1*n1;  
		GetRhsVar(2,"S",&m2,&n2,&Str2);
		mn2 = m2*n2;
		strcat(Str[0] ,Str2[0]);
		if (mn>1) {
			for (x=1;x<mn-1;++x){
				if (strlen(Str2[0])==0) {
					Scierror(999, "2th argument must not be an empty string");
					return 1;
				}
				strcat(Str[0],Str[x]);
				strcat(Str[0] ,Str2[0]);
			}
			strcat(Str[0] ,Str[mn-1]);
		}
		numRow   = 1 ;
		numCol   = 1 ;
		CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str ) ;
		LhsVar(1) = Rhs+1 ; 
 }
  C2F(putlhsvar)();
  return 0;
}
