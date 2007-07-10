/*------------------------------------------------------------------------*/
/* File: sci_stripblanks1.c                                               */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : strips leading and trailing blanks (and tabs) of strings  
          txt=stripblanks(txt[,tabs])   
          Parameters 
          txt : string or matrix of strings 
          tabs : if TRUE then tabs are also stripped (default value is FALSE)*/                                    
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
int C2F(sci_stripblanks1) _PARAMS((char *fname,unsigned long fname_len))
{
	  char **Str,**Str3; /*@ Str is the input string matrix, Str3 is the output string matrix*/
	  int numRow;        /*@ The row number of the output string matrix*/
      int numCol;        /*@ The col number of the output string matrix*/
	  int m1,n1,mn,m2,n2,i,l2,n4=0;
	  Rhs = Max(0, Rhs);
	  CheckRhs(1,4);
      Str3=(char**)MALLOC(sizeof(char*)*(50*50));	  
      GetRhsVar(1,"S",&m1,&n1,&Str);
	  mn = m1*n1;  
	  if (Rhs >= 2) 
	  {
		GetRhsVar(2,"i",&m2,&n2,&l2);
		n4=*istk(l2);   /*@ if n4==0 strips leading and trailing blanks , or strips leading and trailing tabs*/
	  }
	  else n4=0;
	  for (i=0;i<m1*n1;i++)  /*@ To malloc a space the same as input string*/
	  {
	 	  Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		  strcpy(Str3[i],"");
	  }
	  stripblanks(Str,Str3,mn,n4); /*@ The stripblank function*/
	  numRow   = m1;
	  numCol   = n1;
	  CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3) ; /*@ Output*/
	  LhsVar(1) = Rhs+1 ; 
	  FREE(Str3);      /*@Free the memory*/
	  C2F(putlhsvar)();
	  return 0;
  }
 


/*-----------------------------------------------------------------------------------*/
