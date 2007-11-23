/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Alan Layec : - initial  revision : 18/04/07 */
/* Allan CORNET scilab module */
/*--------------------------------------------------------------------------*/
#include "sci_get_fftw_wisdom.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/

/* Return text of fftw wisdom
 *
 * Scilab Calling sequence :
 *   -->tt=get_fftw_wisdom();
 *
 * Input : Nothing
 *
 * Output : a scilab string matrix
 *
 */
int sci_get_fftw_wisdom __PARAMS((char *fname,unsigned long fname_len))
{
  int n1,i,j,k,len;
  char *Str;
  char **Str1=NULL;

  CheckRhs(0,0);
  CheckLhs(0,1);

  Str=call_fftw_export_wisdom_to_string();

  n1 = 0; j = 0;
  for(i = 0; i < (int)strlen(Str); i++) {
   if (Str[i] == '\n') {
     n1++;
     if ((Str1 = (char **)REALLOC(Str1,sizeof(char *)*n1)) == NULL) {
       Scierror(999,_("%s: Memory allocation error\n"),
                    fname);
       return(0);
     }
     len = i-j;
     if ((Str1[n1-1] = (char *)MALLOC(sizeof(char)*(len+1))) == NULL) {
       Scierror(999,_("%s: Memory allocation error\n"),
                    fname);
       return(0);
     }
     for(k = 0; k < len;k++) {
      Str1[n1-1][k] = Str[k+j];
     }
     Str1[n1-1][len] = '\0';
     j = i+1;
    }
  }

  n1++;
  if ((Str1 = (char **)REALLOC(Str1,sizeof(char *)*n1)) == NULL) {
     Scierror(999,_("%s: Memory allocation error\n"),
                  fname);
     return(0);
  }
  if ((Str1[n1-1] = (char *)MALLOC(sizeof(char))) == NULL) {
     Scierror(999,_("%s: Memory allocation error\n"),
                  fname);
     return(0);
  }
  Str1[n1-1][0] = '\0';

  CreateVarFromPtr(1,MATRIX_OF_STRING_DATATYPE, &n1, (j=1,&j), Str1);

  LhsVar(1)=1;

  PutLhsVar();

  for (i = 0; i < n1; i++) FREE(Str1[i]);
  FREE(Str1);

  return(0);
}
/*--------------------------------------------------------------------------*/
