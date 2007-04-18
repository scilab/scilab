/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Alan Layec :  - initial revision  : 30/09/06 fftw3 toolbox*/
/*               - revision : 18/04/07 */
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_fftw_flags.h"
#include "fftw_utilities.h"
#include "MALLOC.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/
extern unsigned cur_fftw_flags;
/*-----------------------------------------------------------------------------------*/
/* fftw_flags function.
*
* Scilab Calling sequence :
*   -->[a,b]=fftw_flags();
* or
*   -->[a,b]=fftw_flags(S);
*
*  a is an int scalar. (the int value of
*                            the flag parameter of fftw)
*  b is a string matrix.(the string values of
*                             the flag parameter of fftw)
*  S is a string matrix or an int or a double scalar
*  given the value(s) of the fftw flag parameter.
*
*  This function gives and set the flag parameter of fftw
*  when creating a new plan.
*  This should be done before calling fftw function.
*  (default is FFTW_ESTIMATE)
*/
int sci_fftw_flags __PARAMS((char *fname,unsigned long fname_len))
{
  /* declaration of variables to store scilab parameters address */
  static int l1, m1, n1;
  SciIntMat M1;
  char **Str1;

  static int l2, m2, n2;

  char **Str3=NULL;
  int len;

  /* please update me ! */
  static int nb_flag=22;
  static char *Str[]= {/* documented flags */
                       "FFTW_MEASURE",
                       "FFTW_DESTROY_INPUT",
                       "FFTW_UNALIGNED",
                       "FFTW_CONSERVE_MEMORY",
                       "FFTW_EXHAUSTIVE",
                       "FFTW_PRESERVE_INPUT",
                       "FFTW_PATIENT",
                       "FFTW_ESTIMATE",

                       /* undocumented beyond-guru flags */
                       "FFTW_ESTIMATE_PATIENT",
                       "FFTW_BELIEVE_PCOST",
                       "FFTW_NO_DFT_R2HC",
                       "FFTW_NO_NONTHREADED",
                       "FFTW_NO_BUFFERING",
                       "FFTW_NO_INDIRECT_OP",
                       "FFTW_ALLOW_LARGE_GENERIC",
                       "FFTW_NO_RANK_SPLITS",
                       "FFTW_NO_VRANK_SPLITS",
                       "FFTW_NO_VRECURSE",
                       "FFTW_NO_SIMD",
                       "FFTW_NO_SLOW",
                       "FFTW_NO_FIXED_RADIX_LARGE_N",
                       "FFTW_ALLOW_PRUNING"};

  static unsigned flagt[]= {/* documented flags */
                            FFTW_MEASURE,
                            FFTW_DESTROY_INPUT,
                            FFTW_UNALIGNED,
                            FFTW_CONSERVE_MEMORY,
                            FFTW_EXHAUSTIVE,
                            FFTW_PRESERVE_INPUT,
                            FFTW_PATIENT,
                            FFTW_ESTIMATE,

                            /* undocumented beyond-guru flags */
                            FFTW_ESTIMATE_PATIENT,
                            FFTW_BELIEVE_PCOST,
                            FFTW_NO_DFT_R2HC,
                            FFTW_NO_NONTHREADED,
                            FFTW_NO_BUFFERING,
                            FFTW_NO_INDIRECT_OP,
                            FFTW_ALLOW_LARGE_GENERIC,
                            FFTW_NO_RANK_SPLITS,
                            FFTW_NO_VRANK_SPLITS,
                            FFTW_NO_VRECURSE,
                            FFTW_NO_SIMD,
                            FFTW_NO_SLOW,
                            FFTW_NO_FIXED_RADIX_LARGE_N,
                            FFTW_ALLOW_PRUNING};

  unsigned flagv=0;

  int i,j;

  CheckRhs(0,1);

  if (Rhs==0) {

  }
  else {
   if (VarType(1)==8) { /* int */
    GetRhsVar(1, "I", &m1, &n1, &M1);
    CheckDims(1,m1,n1,1,1);
    cur_fftw_flags=((int *)M1.D)[0];
   }
   else if (VarType(1)==1) { /* double */
    GetRhsVar(1, "d", &m1, &n1, &l1);
    CheckDims(1,m1,n1,1,1);
    cur_fftw_flags=(int)*stk(l1);
   }
   else if (VarType(1)==10) { /* string */
    GetRhsVar(1,"S",&m1,&n1,&Str1);

    for (j=0;j<m1*n1;j++) {
     for (i=0;i<nb_flag;i++) {
      if (strcmp(Str1[j],Str[i])==0) break;
     }
     if (i==nb_flag) {
      Scierror(999,"%s: "
                   "Bad flag %s for Rhs(1) parameter\n",
                  fname,Str1[j]);
      return(0);
     }
     else {
      if (i>0) flagv=flagv|(1U << (i-1));
     }
    }
    cur_fftw_flags = flagv;
    /* FreeRhsSVar(Str1);*/
    if (Str1) for (i = 0; i<m1*n1 ;i++) {
      if (Str1[i]) {
         FREE(Str1[i]);
         Str1[i]=NULL;
      }
    }
    if (Str1) {FREE(Str1); Str1=NULL;}
   }

   else {
    Scierror(53,"%s: "
                "Bad type for Rhs(1) parameter\n",
                fname);
    return(0);
   }
  }

  /* return value of Sci_Plan.flags in position 2 */
  m2 = 1;
  n2 = m2;
  l2 = I_INT32;
  /*CreateVar(2,"I",&m2,&n2,&l2);*/
  CreateVar(Rhs+2,"I",&m2,&n2,&l2);
  *istk(l2)=(int) cur_fftw_flags;

  /*Test for only FFTW_MEASURE*/
  if(cur_fftw_flags==0) {
   j = 1;
   if ((Str3 = (char **)MALLOC(sizeof(char *))) == NULL) {
     Scierror(999,"%s: "
                  "Memory allocation error\n",
                  fname);
     return(0);
   }
   len = (int)strlen(Str[0]);
   if ((Str3[0] = (char *)MALLOC(sizeof(char)*(len+1))) == NULL) {
     Scierror(999,"%s: "
                  "Memory allocation error\n",
                  fname);
     return(0);
   }
   sprintf(Str3[0],"%s",Str[0]);
  }
  else {
   j = 0;
   for (i = 1;i < nb_flag; i++) {
    if((cur_fftw_flags&flagt[i])==flagt[i]) {
     j++;
     if ((Str3 = (char **)REALLOC(Str3,sizeof(char *)*j)) == NULL) {
       Scierror(999,"%s: "
                    "Memory allocation error\n",
                    fname);
       return(0);
     }
     len = (int)strlen(Str[i]);
     if ((Str3[j-1] = (char *)MALLOC(sizeof(char)*(len+1))) == NULL) {
       Scierror(999,"%s: "
                    "Memory allocation error\n",
                    fname);
       return(0);
     }
     sprintf(Str3[j-1],"%s",Str[i]);
    }
   }
  }

  n1=1;
  /*CreateVarFromPtr( 3, "S", &j, &n1, Str3);*/
  CreateVarFromPtr( Rhs+3, "S", &j, &n1, Str3);

  /*LhsVar(1)=2;*/
  /*LhsVar(2)=3;*/
  LhsVar(1)=Rhs+2;
  LhsVar(2)=Rhs+3;
  PutLhsVar();

  for (i=0;i<j;i++) FREE(Str3[i]);
  FREE(Str3);

  return(0);
}
/*-----------------------------------------------------------------------------------*/ 
