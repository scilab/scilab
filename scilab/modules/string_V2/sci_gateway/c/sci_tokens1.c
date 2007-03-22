/*------------------------------------------------------------------------*/
/* File: sci_strindex1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : search position of a character string in an other string
          using regular express .                                         */
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


/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_tokens1) _PARAMS((char *fname,unsigned long fname_len))
{
//char typ = '*';
//
//  char **Str,**Str2;
//  static char def_sep[] ="";
//  char *sep = def_sep;
//  static int un=1;
//  int x1,m1,n1,mn,mn2,i,j,k,m2,n2,nchars=0;
//  unsigned x;
//  int lenth=0;
//  int boolen;
//  int outIndex = 0 ;
//  char ebuf[128];
//
//
//  int  z, lno = 0, cflags = 0;
//  char *values[100];
//  int colValues=0;
//  int rowValues=0;
//
//  int numRow   = 1 ;
//  int numCol   = 1 ;
// 
//  regmatch_t pm[10];
//
//
//
//  regex_t **out1;
//  Rhs = Max(0, Rhs);
//
//  CheckRhs(1,2);
//
//  
//  switch ( VarType(1)) {
//  case 10 :
//    GetRhsVar(1,"S",&m1,&n1,&Str);
//    mn = m1*n1;  
//	GetRhsVar(2,"S",&m2,&n2,&Str2);
//    mn2 = m2*n2;  
//
//	for (i=0;i<mn;++i){
//		if (Str[0][i]==Str2[0][0]) { 
//			colValues++;
//			rowValues=0;
//		}
//		else {
//
//    	values[colValues][rowValues++]=Str[0][i];
//		}
//	}
//
//   
//
//	
//	
//		
//	
//	
//	
//	for (x=0;x<mn2;++x){
//		z = regexec(out1[x], Str[0], nmatch, pm, 0);
//		if (z == REG_NOMATCH) { 
//			int outIndex2= Rhs +x+1 ;
//			int numRow   = 1 ;
//            int numCol   = 1 ;
//            int outIndex = 0 ;
//            CreateVar(Rhs+1+x,"c",&numRow,&numCol,&outIndex);
//  			LhsVar(x+1) = outIndex2 ;
//			
//			continue;
//		}
//		for (x1 = 0; x1 < nmatch && pm[x1].rm_so != -1; ++ x1) {
//		
//          
//			values[nbValues++]=pm[x1].rm_so+1;
//			position[nbposition++]=x+1;
//		
//
//			
//			
//
//        }     
//
//	}
//
//		numRow   = 1        ;
//        outIndex = 0        ;
//  
//        CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
//        for ( i = 0 ; i < nbposition ; i++ )
//  {
//    stk(outIndex)[i] = (double)values[i] ;
//  }
//		LhsVar(1) = Rhs+1 ;
//
//
//		numRow   = 1        ;
//        outIndex = 0        ;
//  
//        CreateVar(Rhs+2,"d",&numRow,&nbposition,&outIndex) ;
//        for ( i = 0 ; i < nbposition ; i++ )
//  {
//    stk(outIndex)[i] = (double)position[i] ;
//  }
//		LhsVar(2) = Rhs+2;	
//		
//
//
//
//    
//   
//  
//  }
  
  C2F(putlhsvar)();
	return 0;
}

/*-----------------------------------------------------------------------------------*/
