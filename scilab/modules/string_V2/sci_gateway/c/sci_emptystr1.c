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
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "returnProperty.h"
#include "machine.h"
#include "MALLOC.h" 
int next[20];
int numRow;
int numCol;
char *_replacedstr;
/*-------------------------------------------------------------------------------------*/

void getnext(char T[],int *next);
int kmp(char S[],char T[],int pos);







static char* newstr(const char*str, unsigned start, unsigned end, char*_replacedstr1)

{

unsigned n = end - start;
unsigned i,j;
static char stbuf1[256];
static char stbuf2[256];
static char stbuf3[256];
static char stbuf4[256];

for (i=0;i<256;++i) {
   stbuf1[i]=0;
   stbuf2[i]=0;
   stbuf3[i]=0;
   stbuf4[i]=0;
}
strncpy(stbuf1, str , start);
strncpy(stbuf2, _replacedstr1,strlen(_replacedstr1));
strncpy(stbuf3, str+end ,strlen(str)-n+strlen(_replacedstr1) );
j=0;
for (i=0;i<strlen(stbuf1);i++) stbuf4[j++]=stbuf1[i];
for (i=0;i<strlen(stbuf2);i++) stbuf4[j++]=stbuf2[i];
for (i=0;i<strlen(stbuf3);i++) stbuf4[j++]=stbuf3[i];

return stbuf4;

}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_emptystr1) _PARAMS((char *fname,unsigned long fname_len))
{
char typ = '*';

  char **Str,**Str2,**Str3,**Str4;
  
  
  
  int x,m1,n1,mn,mn2,mn3,m2,n2,m3,i,n3,m4,n4,l1,l4,l2=0;
  unsigned x1;
  
  
  
  
  char ebuf[128];
  char *lbuf;
  const size_t nmatch = 10;
  char *pattern;
  int w;
  int pos=0;
  int  z, cflags = 0;
  regmatch_t pm[10];
  regex_t *out1;
  Rhs = Max(0, Rhs);
  //CheckRhs(0,5);
   if (Rhs < 1) { 
		int m1=0;
		int n1=0;
		int l1=0;
		
		CreateVar(1,"c",  &m1, &n1, &l1);
		LhsVar(1)=1;
		C2F(putlhsvar)();
	    return 0;
   }
 

   if (Rhs ==1) {
	  
	GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
	l1=0;
	Str4=(char**)MALLOC(sizeof(char*)*(m1*n1));
	for (i=0;i<m1*n1;i++)
		{
			Str4[i]=(char*)MALLOC(sizeof(char*)*(1));
			strcpy(Str4[i],"");
		}
	CreateVarFromPtr(1,"S",&m1, &n1, Str4);
	LhsVar(1)=1;
   }
   if (Rhs == 2){
		GetRhsVar(1,"i",&m1,&n1,&l1);
		m4=*istk(l1);
		GetRhsVar(2,"i",&m2,&n2,&l2);
		n4=*istk(l2);
		if (m4*n4!=0) Str4=(char**)MALLOC(sizeof(char*)*(m4*n4));
		for (i=0;i<m4*n4;i++)
		{
			Str4[i]=(char*)MALLOC(sizeof(char*)*(1));
			strcpy(Str4[i],"");
		}
		CreateVarFromPtr(1,"S",&m4, &n4, Str4);
		LhsVar(1)=1;
   }
    

  //}
	


  C2F(putlhsvar)();
  return 0;
}

/*-----------------------------------------------------------------------------------*/
