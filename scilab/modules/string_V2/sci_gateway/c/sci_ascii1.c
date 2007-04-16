/*------------------------------------------------------------------------*/
/* File: sci_strsubst1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : substitute a character string by another in a character string
          using regular express .                                         */
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

/*-----------------------------------------------------------------------------------*/
int C2F(sci_ascii1) _PARAMS((char *fname,unsigned long fname_len))
{


  char **Str,**Str3;
  char typ = '*';


  
  
  
  int x,y,m1,n1,mn,m2,n2,mn3,i,l1,l2,m4,n4,l4=0;
  int values[100];

  int nbValues=0;
  int numRow   = 1        ;
  int outIndex = 0        ;

  unsigned *m;
  
  
  




  
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);

  switch ( GetType(1)) {
  case sci_matrix :
	GetRhsVar(1,"S",&m1,&n1,&Str);
	mn = m1*n1;  
	for (x=0;x<mn;x++)
		for (y=0;y<strlen(Str[x]);y++)
			values[nbValues++]=Str[x][y];
	 
	CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
	for ( i = 0 ; i < nbValues ; i++ )
	{
		stk(outIndex)[i] = (double)values[i] ;
	}
	
	
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();
	return 0;
 
	
  }
    
}

/*-----------------------------------------------------------------------------------*/
