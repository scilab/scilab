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
int next[20];
void getnext(char T[],int *next);
int kmp(char S[],char T[],int pos);
/**
 * The KMP method to search in a string.
 * @S[] is  the input of the main string.
 * @T[] is  the input of substring.
 * @return  the start point of the substring or 0.
 */
int kmp(char S[],char T[],int pos){
     int i,j,lenS,lenT;
     lenS=strlen(S);          /*The length of the main string*/
     lenT=strlen(T);          /*The length of the substring*/
     i=pos;                   /*The start point*/
     j=0;
	 while(i<lenS && j<lenT){
		 if(j==-1||S[i]==T[j]) {
              i++;           /*Compare with the char next*/
              j++;
         }
         else
           j=next[j];    /*Using the next pattern to move right*/

     }
       if(j>=lenT) 
           return(i-lenT+1); 
       else 
           {            /*Should give an error message*/
            return(0);
            
           }

    }

/**
 * To get the next value of the substring of the KMP method.
 * @*next is the pointer to the next value.
 * @T[] is  the input of substring.
 */
void getnext(char T[], int *next1){  /*To get the next value of the substring*/
        int i,j,lenT;
       
		i=0;
		j=-1;
        lenT=strlen(T);
        *(next1)=-1;         
		while(i<lenT){
			if(j==-1||T[i]==T[j]){  
                  ++i;
                  ++j;
                  *(next1+i)=j;
            }
            else 
              j=*(next1+j);

                 
        }
}

/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
int C2F(sci_grep1) _PARAMS((char *fname,unsigned long fname_len))
{
	char typ = '*';
	char **Str,**Str2;
	int y,x,m1,n1,mn,mn2,i,m2,n2,m3,n3,l3=0;
	unsigned x1;
    int outIndex = 0 ;
    char ebuf[128];
	const size_t nmatch = 10;
    char *pattern[100];
    int  z, cflags = 0;
    int values[10];
    int nbValues=0;
    int position[10];
    int nbposition=0;
    int numRow   = 1 ;
	int w;
    int pos=0;
	regmatch_t pm[10];
	regex_t *out1[100];
	Rhs = Max(0, Rhs);
	CheckRhs(1,3);
	CheckLhs(1,2);
	//next=(int*)MALLOC(sizeof(int));
	switch ( VarType(1)) {
		case 10 :
			GetRhsVar(1,"S",&m1,&n1,&Str);
			mn = m1*n1;  
			GetRhsVar(2,"S",&m2,&n2,&Str2);
			mn2 = m2*n2;  
			if (Rhs >= 3) {
				GetRhsVar(3,"c",&m3,&n3,&l3);
				if ( m3*n3 != 0) 
					typ = cstk(l3)[0];
				if (typ == 'r' ) {            /*When we use the regexp;*/
					for (i=0;i<mn2;++i){
						pattern[i]=Str2[i];
						out1[i]=(regex_t *)malloc(sizeof(regex_t));
						z = regcomp(out1[i], pattern[i], cflags);
						if (z != 0){
							regerror(z, *out1, ebuf, sizeof(ebuf));
							Scierror(999, "%s: pattern '%s' \n", ebuf, pattern);
							return 1;
						}
					}
					for (y=0;y<mn;++y){
						for (x=0;x<mn2;++x){
							z = regexec(out1[x], Str[y], nmatch, pm, 0);
							if (z == REG_NOMATCH) { 
								int outIndex2= Rhs +x+1 ;
							   int numCol   = 1 ;
								outIndex = 0 ;
								numRow  =1 ;
                        CreateVar(Rhs+1+x,"c",&numRow,&numCol,&outIndex);
  								LhsVar(x+1) = outIndex2 ;
								continue;
							}
							for (x1 = 0; x1 < nmatch && pm[x1].rm_so != -1; ++ x1) {
								values[nbValues++]=y+1;
								position[nbposition++]=x+1;
							}     
						}
					}
				}/* end of typ=='r'*/
			}/* end of Rhs>=3*/
			else { /* without using the regexp*/
				for (y=0;y<mn;++y){
					for (x=0;x<mn2;++x){
						if (strlen(Str2[x])==0) {
							 Scierror(999, "2th argument must not be an empty string");
						   return 1;
						}
						getnext(Str2[x],next); 
						w=kmp(Str[y],Str2[x],pos);
						if (w !=0) { 	       
							values[nbValues++]=y+1;
							position[nbposition++]=x+1;
						} 	 
					}
				}
			} 
			numRow   = 1        ;
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
			FREE(out1);
			FREE(next);
	}
  C2F(putlhsvar)();
	return 0;
}

/*-----------------------------------------------------------------------------------*/
