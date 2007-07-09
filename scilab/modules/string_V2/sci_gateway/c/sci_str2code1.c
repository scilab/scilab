/*------------------------------------------------------------------------*/
/* File: sci_str2code1.c                                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : This function return scilab integer codes associated with a 
          character string                                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h" 
#include "src_string.h"
/*-------------------------------------------------------------------------------------*/
int C2F(sci_str2code1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str;
  int m1,n1,mn,i=0;
  int values[100];
  int nbValues=0;
  int numRow   = 1        ;
  int outIndex = 0        ;
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);
  switch ( VarType(1)) {
	  case 10 :
	  GetRhsVar(1,"S",&m1,&n1,&Str);
	  mn = m1*n1; 
	  str2code(Str,values,&nbValues);
	  //  for (x=0;x<=62;x++)       /* To look up in the table */
			//if (Str[0][0] == m5[x]) 
	 	//	{
			//	key=1;
   //             values[nbValues++]=x;
			//}	
			//if (key==0) 
			//	for (x=0;x<=62;x++)
			//		if (Str[0][0] == m6[x]) 
	 	//			{
			//			key=1;
			//			values[nbValues++]=-x;
			//		}	   
		CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;    /*Output*/
		for ( i = 0 ; i < nbValues ; i++ )
		{
			stk(outIndex)[i] = (double)values[i] ;
		}
		LhsVar(1) = Rhs+1 ;
		C2F(putlhsvar)();
  }
  return 0;
}

/*-----------------------------------------------------------------------------------*/
