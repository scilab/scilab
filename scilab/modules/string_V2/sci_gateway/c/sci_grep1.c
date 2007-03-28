/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include <ctype.h> 
#include "returnProperty.h"
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "MALLOC.h" /* MALLOC */
static char* substr(const char*str, unsigned start, unsigned end)

{

unsigned n = end - start;

static char stbuf[256];

strncpy(stbuf, str + start, n);

stbuf[n] = 0;

return stbuf;

}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_grep1) _PARAMS((char *fname,unsigned long fname_len))
{
char typ = '*';

  char **Str,**Str2;
  static char def_sep[] ="";
  char *sep = def_sep;
  static int un=1;
  int y,x,m1,n1,mn,mn2,i,m2,n2,nchars=0;
  unsigned x1;
  int lenth=0;
  int lenthrow=1;
  int lenthcol=1;
  int outIndex = 0 ;
  char ebuf[128];
 
  const size_t nmatch = 10;
  char *pattern[100];
  int  z, lno = 0, cflags = 0;
  int values[10];
  int nbValues=0;
  int position[10];
  int nbposition=0;
  int numRow   = 1 ;
  int numCol   = 1 ;
 
  regmatch_t pm[10];



  regex_t *out1[100];
  Rhs = Max(0, Rhs);

  CheckRhs(1,2);
  CheckLhs(1,2);
  
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,"S",&m1,&n1,&Str);
    mn = m1*n1;  
	GetRhsVar(2,"S",&m2,&n2,&Str2);
    mn2 = m2*n2;  
	
	for (i=0;i<mn2;++i){
		pattern[i]=Str2[i];
		out1[i]=(regex_t *)malloc(sizeof(regex_t));
        z = regcomp(out1[i], pattern[i], cflags);
	    if (z != 0){
			regerror(z, out1, ebuf, sizeof(ebuf));
			Scierror(999, "%s: pattern '%s' \n", ebuf, pattern);
			return 1;

		}
	}
	
	for (y=0;y<mn;++y){
		for (x=0;x<mn2;++x){
			z = regexec(out1[x], Str[y], nmatch, pm, 0);
			if (z == REG_NOMATCH) { 
				int outIndex2= Rhs +x+1 ;
				int numRow   = 1 ;
	            int numCol   = 1 ;
	            int outIndex = 0 ;
	           CreateVar(Rhs+1+x,"c",&numRow,&numCol,&outIndex);
  				LhsVar(x+1) = outIndex2 ;
			
				continue;
			}
			for (x1 = 0; x1 < nmatch && pm[x1].rm_so != -1; ++ x1) {
		
          
				values[nbValues++]=y+1;
				position[nbposition++]=x+1;
			//values[nbValues++]=pm[x1].rm_eo;

			
			

			}     

		}

	}
     		numRow   = 1        ;
			outIndex = 0        ;
  
			CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
			for ( i = 0 ; i < nbposition ; i++ )
	  {
		stk(outIndex)[i] = (double)values[i] ;
	 }
			LhsVar(1) = Rhs+1 ;


			numRow   = 1        ;
			outIndex = 0        ;
  
        CreateVar(Rhs+2,"d",&numRow,&nbposition,&outIndex) ;
        for ( i = 0 ; i < nbposition ; i++ )
  {
    stk(outIndex)[i] = (double)position[i] ;
  }
		LhsVar(2) = Rhs+2;	
		



    
   
  
  }
  
  C2F(putlhsvar)();
	return 0;
}

/*-----------------------------------------------------------------------------------*/
