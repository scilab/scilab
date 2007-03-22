/*------------------------------------------------------------------------*/
/* File: sci_strsubst1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : substitute a character string by another in a character string
          using regular express .                                         */
/*------------------------------------------------------------------------*/
#include <string.h>
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
#ifdef _MSC_VER
#pragma comment(lib,"../../../../pcre.lib")
#endif
/*-------------------------------------------------------------------------------------*/

static char* newstr(const char*str, unsigned start, unsigned end, char*newstr1)

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
strncpy(stbuf2, newstr1,strlen(newstr1));
strncpy(stbuf3, str+end ,strlen(str)-n+strlen(newstr1) );
j=0;
for (i=0;i<strlen(stbuf1);i++) stbuf4[j++]=stbuf1[i];
for (i=0;i<strlen(stbuf2);i++) stbuf4[j++]=stbuf2[i];
for (i=0;i<strlen(stbuf3);i++) stbuf4[j++]=stbuf3[i];

return stbuf4;

}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_strsubst1) _PARAMS((char *fname,unsigned long fname_len))
{
char typ = '*';

  char **Str,**Str2,**Str3;
  static char def_sep[] ="";
  char *sep = def_sep;
  static int un=1;
  int x,m1,n1,mn,mn2,mn3,m2,n2,m3,n3,nchars=0;
  unsigned x1;
  int lenth=0;
  int lenthrow=1;
  int lenthcol=1;
  int outIndex = 0 ;
  char ebuf[128];
  char *lbuf;
  const size_t nmatch = 10;
  char *pattern;
  int  z, lno = 0, cflags = 0;
  regmatch_t pm[10];
  regex_t *out1;
  Rhs = Max(0, Rhs);
  CheckRhs(1,3);

  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
	GetRhsVar(2,"S",&m2,&n2,&Str2);
    mn2 = m2*n2;  
	GetRhsVar(3,"S",&m3,&n3,&Str3);
    mn3 = m3*n3; 
	lbuf=*Str;
	pattern=*Str2;
	out1=(regex_t *)malloc(sizeof(regex_t));
    z = regcomp(out1, pattern, cflags);
	if (z != 0){
		regerror(z, out1, ebuf, sizeof(ebuf));
		Scierror(999, "%s: pattern '%s' \n", ebuf, pattern);
		return 1;
	}

	for (x = 0; x < mn;++x){
		z = regexec(out1, Str[x], nmatch, pm, 0);
		if (z == REG_NOMATCH) { 
			int outIndex2= Rhs +x+1 ;
			int numRow   = 1 ;
            int numCol   = 1 ;
            int loutIndex = 0 ;
            CreateVar(Rhs+1+x,"c",&numRow,&numCol,&loutIndex);
  			LhsVar(x+1) = outIndex2 ;
			continue;
		}
		for (x1 = 0; x1 < nmatch && pm[x1].rm_so != -1; ++ x1) {
		    int outIndex2= Rhs +x+1 ;
			int numRow   = 1 ;
            int numCol   = strlen( newstr(Str[x], pm[x1].rm_so, pm[x1].rm_eo,*Str3)) ;
            int loutIndex = 0 ;
			CreateVar(Rhs+1+x,"c",&numRow,&numCol,&loutIndex);
            strncpy(cstk(loutIndex),newstr(Str[x], pm[x1].rm_so, pm[x1].rm_eo,*Str3), numCol);
			LhsVar(x+1) = outIndex2 ;

        }     
    }
  }
  C2F(putlhsvar)();
  return 0;
}

/*-----------------------------------------------------------------------------------*/
