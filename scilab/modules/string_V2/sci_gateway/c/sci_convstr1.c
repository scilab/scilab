/*------------------------------------------------------------------------*/
/* File: sci_convstr1.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : converts the matrix of strings  str-matrix  into lower case (for*/  
/*          "l"  ;default value) or upper case (for  "u" ).               */
/*        [y]=convstr(str-matrix, ["flag"])                               */
/*              str-matrix, y : matrices of strings                       */
/*              "flag"  : string ( "u"  for upper or  "l"  for lower      */
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
int C2F(sci_convstr1) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str3;
  char typ = '*';
  int m1,n1,mn,i,m4,n4,l4=0;
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
	
	convstr(Str,Str3,typ,mn);
	//for (x=0;x<mn;x++){
	//	for (y=0;y<strlen(Str[x]);y++){
	//		if (typ=='u') {     /*converts the matrix of strings  str-matrix  into upper case */
	//			if    ( Str[x][y]>=97 && Str[x][y]<=122 ) 
 //           			Str3[x][y]=Str[x][y]-32		;
	//			else Str3[x][y]=Str[x][y];
	//		}
	//		else if (typ=='l') { /*converts the matrix of strings  str-matrix  into lower case */
	//			if    ( Str[x][y]>=65 && Str[x][y]<=90 ) 
 //           			Str3[x][y]=Str[x][y]+32		;
 //               else Str3[x][y]=Str[x][y];
	//		}
	//	}
 //       Str3[x][y]=0;
	//}


	numRow   = m1;  /*Output */
    numCol   = n1 ;
    CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3 ) ;
	LhsVar(1) = Rhs+1 ; 
    C2F(putlhsvar)();
    FREE(Str3);
  }
    return 0;
}

/*-----------------------------------------------------------------------------------*/
