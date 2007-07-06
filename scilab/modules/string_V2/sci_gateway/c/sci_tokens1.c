/*------------------------------------------------------------------------*/
/* File: sci_tokens1.c                                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : returns the tokens of a character string.  
          T=tokens(str [,delimiter])   
		  str : a character string. The string where to search the tokens. 
          delimiter : (optional) a character or a vector of characters. 
		              The tokens delimeters. 
		  T : column vector of found tokens 
		                                                                  */
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
int C2F(sci_tokens1) _PARAMS((char *fname,unsigned long fname_len))
{

  char **Str,**Str2,**Str3;
  int m1,n1,m2,n2,mn2,mn,i=0;
  int u=0;
  int w=0;
  int numRow;
  int numCol;
  Rhs = Max(0, Rhs);
  CheckRhs(1,2);
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
	if (Rhs == 2) {
		GetRhsVar(2,"S",&m2,&n2,&Str2);
		mn2=m2*n2;
	}
	else { 
		Str2=(char**)MALLOC(sizeof(char*)*(20*20));
		Str2[0]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str2[0],"");
		Str2[0][0]=32;
		m2=1;
		n2=1;
		mn2=1;
	}
	Str3=(char**)MALLOC(sizeof(char*)*(20*20));
	for (i=0;i<20*20;i++)
	{
		Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str3[i],"");
	}
	tokens(Str,Str2,Str3,&u,&w,mn,mn2);
	/*for (x=0;x<mn;x++){
		for (y=0;y<strlen(Str[x]);y++){
				for (i=0; i<mn2; i++)  if (Str[x][y]==Str2[i][0]) bo=1;
				if    (bo==0) Str3[u][w++]=Str[0][y];
				else {
					Str3[u][w]=0;
					u++;
					w=0;
				}
				bo=0;
		}
	}*/
    numRow   = u+1;  /*Output */
    numCol   = 1 ;
    CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3 ) ;
    LhsVar(1) = Rhs+1 ; 
    C2F(putlhsvar)();
    FREE(Str3);
  }
    return 0;
}
