
/*------------------------------------------------------------------------*/
/* File: sci_strindex1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu                                                      */
/* desc : search position of a character string in an other string
          using regular express .                                         */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h> 
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "returnProperty.h"
#include "machine.h"
#include "MALLOC.h" 
int next[20];




int C2F(sci_strindex1) _PARAMS((char *fname,unsigned long fname_len))
{
	char typ = '*';
    char **Str,**Str2;
    static char def_sep[] ="";
    char *sep = def_sep;
    static int un=1;
    int x,m1,n1,mn,mn2,i,m2,n2,m3,n3,l3,nchars=0;
    unsigned x1;
    int lenth=0;
    int lenthrow=1;
    int lenthcol=1;
    int outIndex = 0 ;
    char ebuf[128];
    char *lbuf;
    const size_t nmatch = 10;
    char *pattern[100];
    int  z, lno = 0, cflags = 0;
    int values[10];
    int nbValues=0;
    int position[10];
    int nbposition=0;
    int numRow   = 1 ;
    int numCol   = 1 ;
    int w;
    int pos=0;
 
    regmatch_t pm[10];
    regex_t *out1[10];
    Rhs = Max(0, Rhs);
    CheckRhs(1,3);
    CheckLhs(1,2);
    
    switch ( VarType(1)) {
		case 10 :
			GetRhsVar(1,"S",&m1,&n1,&Str);
			mn = m1*n1;  
			if (mn != 1) {
				/*give an error message that the first is not correct.*/
				return 1;
			}
			GetRhsVar(2,"S",&m2,&n2,&Str2);
			mn2 = m2*n2;  
			lbuf=*Str;
			if (Rhs >= 3) {
				GetRhsVar(3,"c",&m3,&n3,&l3);
				if ( m3*n3 != 0) 
					typ = cstk(l3)[0];
				if (typ == 'r' ) {            /*When we use the regexp;*/
					for (i=0;i<mn2;++i){      /*  To compile the regexp pattern;*/
						pattern[i]=Str2[i];
						out1[i]=(regex_t *)malloc(sizeof(regex_t));
						z = regcomp(out1[i], pattern[i], cflags);
						if (z != 0){
							regerror(z, out1[i], ebuf, sizeof(ebuf));
							Scierror(999, "%s: pattern '%s' \n", ebuf, pattern);
							return 1;
						}
					}
	
	
					for (x=0;x<mn2;++x){
						z = regexec(out1[x], Str[0], nmatch, pm, 0);
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
							values[nbValues++]=pm[x1].rm_so+1;
							position[nbposition++]=x+1;
						}     

					}

	        

	           
			  }
			} 
			else {       /* When we do not use the regexp.*/
				for (x=0;x<mn2;++x){
					if (strlen(Str2[x])==0) {
						 Scierror(999, "2th argument must not be an empty string");
				       return 1;
					}
					getnext(Str2[x],next); 
					w=kmp(*Str,Str2[x],pos);
					if (w !=0) { 	       
						values[nbValues++]=w;
						position[nbposition++]=x+1;
					} 	 
				}
			} 
		
	    
			numRow   = 1        ;/* Output values[] and position[]*/
			outIndex = 0        ;
  
			CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
			for ( i = 0 ; i < nbposition ; i++ ){
				  stk(outIndex)[i] = (double)values[i] ;
			}
			LhsVar(1) = Rhs+1 ;
			numRow   = 1        ;
			outIndex = 0        ;
			CreateVar(Rhs+2,"d",&numRow,&nbposition,&outIndex) ;
			for ( i = 0 ; i < nbposition ; i++ ){
				stk(outIndex)[i] = (double)position[i] ;
			}
			LhsVar(2) = Rhs+2;	
		
	}
  

  C2F(putlhsvar)();
	return 0;
}

/*-----------------------------------------------------------------------------------*/

