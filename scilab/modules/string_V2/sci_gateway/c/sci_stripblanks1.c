/*------------------------------------------------------------------------*/
/* File: sci_stripblanks1.c                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
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
int numRow;
int numCol;
char *_replacedstr;
static char* substr(const char*str, unsigned start, unsigned end)

	{

	unsigned n = end - start;

	char *stbuf;
       stbuf=(char*)MALLOC(sizeof(char*)*(1));

	strncpy(stbuf, str + start, n);

	stbuf[n] = 0;

	return stbuf;

	}

/*-------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
int C2F(sci_stripblanks1) _PARAMS((char *fname,unsigned long fname_len))
{
	  char **Str,**Str3;
	  int x,m1,n1,mn,m2,n2,i,l2,n4,l1=0;
	  Rhs = Max(0, Rhs);
	  CheckRhs(1,4);
	  switch ( VarType(1)) {
	  case 10 :
			GetRhsVar(1,"S",&m1,&n1,&Str);
			mn = m1*n1;  
			if (Rhs >= 2) {
				GetRhsVar(2,"i",&m2,&n2,&l2);
				n4=*istk(l2);
			}
			else n4=0;
			Str3=(char**)MALLOC(sizeof(char*)*(m1*n1));
			for (i=0;i<m1*n1;i++)
			{
				Str3[i]=(char*)MALLOC(sizeof(char*)*(1));
				strcpy(Str3[i],"");
			}
			for (x=0;x<mn;x++){
			
					if (n4==0) {                 /*strips leading and trailing blanks*/
						l2=strlen(Str[x])-1;
						while (l2>0 &&  Str[x][l2]==' ')  --l2;
						l1=0;
						while (l1<l2 &&  Str[x][l1]==' ') ++l1;
						if (l1<=l2 && Str[x][l1]!=' ') {
							_replacedstr=substr(Str[x], l1, l2+1 );
							strcpy(Str3[x],_replacedstr); 
						}
					}
					else  {                    /*strips leading and trailing tabs*/
						l2=strlen(Str[x])-1;
						while (l2>0 &&  Str[x][l2]==9)  --l2;
						l1=0;
						while (l1<l2 &&  Str[x][l1]==9) ++l1;
						if (l1<=l2 && Str[x][l1]!=9 && Str[x][l1]!=' ') {
							_replacedstr=substr(Str[x], l1, l2+1 );
							strcpy(Str3[x],_replacedstr); 
						}
					}
			}
	  }
		numRow   = m1;
		numCol   = n1;
		CreateVarFromPtr( Rhs+1, "S", &numRow, &numCol, Str3) ;
		LhsVar(1) = Rhs+1 ; 
		FREE(Str3);
	  C2F(putlhsvar)();
	  return 0;
  }
 


/*-----------------------------------------------------------------------------------*/
