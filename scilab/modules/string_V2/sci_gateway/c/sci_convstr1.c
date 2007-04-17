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
int C2F(sci_convstr1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str3;
  char typ = '*';
  int x,y,m1,n1,mn,m2,n2,mn3,i,l2,m4,n4,l4=0;
  unsigned *m;
  Rhs = Max(0, Rhs);
  CheckRhs(1,4);
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
    GetRhsVar(2,"c",&m4,&n4,&l4);
    if ( m4*n4 != 0) 
	     typ = cstk(l4)[0]; 
	Str3=(char**)MALLOC(sizeof(char*)*(m1*n1));
	for (i=0;i<m1*n1;i++)
	{
		Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
		strcpy(Str3[i],"");
	}
	for (x=0;x<mn;x++){
		for (y=0;y<strlen(Str[x]);y++){
			if (typ=='u') {
				if    ( Str[x][y]>=97 && Str[x][y]<=122 ) 
            			Str3[x][y]=Str[x][y]-32		;
				else Str3[x][y]=Str[x][y];
			}
			else if (typ=='l') { 
				if    ( Str[x][y]>=65 && Str[x][y]<=90 ) 
            			Str3[x][y]=Str[x][y]+32		;
                else Str3[x][y]=Str[x][y];
			}
		}
        Str3[x][y]=0;
	}
	numRow   = m1;
    numCol   = n1 ;
    CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3 ) ;
	LhsVar(1) = Rhs+1 ; 
    C2F(putlhsvar)();

  }
    return 0;
}

/*-----------------------------------------------------------------------------------*/
