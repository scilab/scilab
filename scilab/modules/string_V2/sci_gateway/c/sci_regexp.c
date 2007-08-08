/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
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
#include "MALLOC.h" /* MALLOC */
static char* substr(const char*str, unsigned start, unsigned end)

{

unsigned n = end - start;

static char stbuf[256];

strncpy(stbuf, str + start, n);

stbuf[n] = 0;

return stbuf;

}
/**
* Scilab regular expression 
* 
* @param fname function
* @param fname_len
* @return ...
*/
int C2F(sci_regexp) _PARAMS((char *fname,unsigned long fname_len))
{
  char **Str,**Str2;
  int x,m1,n1,mn,mn2,m2,n2=0;
  unsigned x1;
  int outIndex = 0 ;
  char ebuf[128];
  int numRow;
  const size_t nmatch = 10;
  char *pattern;
  int  z,cflags = 0;
  int stpoint[10];
  int endpoint[10];
  int nbstpoint=0;
  int nbendpoint=0;
  regmatch_t pm[10];
  regex_t *out1=NULL;
  Rhs = Max(0, Rhs);


  
  switch ( VarType(1)) {
  case 10 :
    GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
    mn = m1*n1;  
	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
    mn2 = m2*n2;  
    pattern=*Str2;

    out1=(regex_t *)malloc(sizeof(regex_t));
    z = regcomp(out1, pattern, cflags);
    if (z != 0){
        regerror(z, out1, ebuf, sizeof(ebuf));
		Scierror(999, "%s: pattern '%s' \n", ebuf, pattern);
		return 1;
	}
	for (x = 0; x < mn;++x){
		z = regexec(out1, Str[0], nmatch, pm, 0);
		if (z == REG_NOMATCH) { 
			int outIndex2= Rhs +x+1 ;
			int numCol   = 1 ;
			numRow   = 1 ;
            outIndex = 0 ;
            CreateVar(Rhs+1+x,STRING_DATATYPE,&numRow,&numCol,&outIndex);
  			LhsVar(x+1) = outIndex2 ;
			
			continue;
		}
		for (x1 = 0; x1 < nmatch && pm[x1].rm_so != -1; ++ x1) {
			
            //CreateVar(Rhs+1+x,STRING_DATATYPE,&numRow,&numCol,&outIndex);
            //strncpy(cstk(outIndex),substr(Str[x], pm[x1].rm_so, pm[x1].rm_eo), numCol);
			stpoint[nbstpoint++]=pm[x1].rm_so+1;
			endpoint[nbendpoint++]=pm[x1].rm_eo;
           	sciReturnRowVectorFromInt(stpoint,nbstpoint);
			LhsVar(x+1) = Rhs +x+1 ;
			

        }     
	}
	
	//numRow   = 1        ;
	//outIndex = 0        ;
	//CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbstpoint,&outIndex);
	//for ( i = 0 ; i < nbstpoint ; i++ ){
	//	stk(outIndex)[i] = (double)stpoint[i] ;
	//}	
	//LhsVar(1) = Rhs+1 ;
	//numRow   = 1        ;
	//outIndex = 0        ;
	//CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbendpoint,&outIndex);
	//for ( i = 0 ; i < nbendpoint ; i++ ){
	//	stk(outIndex)[i] = (double)endpoint[i] ;
	//}	
	//LhsVar(2) = Rhs+2 ;	
		



    
   
  
  }
  
  C2F(putlhsvar)();
	return 0;
}

/*-----------------------------------------------------------------------------------*/
