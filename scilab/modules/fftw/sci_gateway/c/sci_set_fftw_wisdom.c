/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Alan Layec : - initial  revision : 18/04/07 */
/* Allan CORNET scilab module */
/*-----------------------------------------------------------------------------------*/
#include "sci_set_fftw_wisdom.h"
#include "callfftw.h"
#include "MALLOC.h"
#include "gw_fftw.h"
/*-----------------------------------------------------------------------------------*/

/* Set fftw wisdom
 *
 * Scilab Calling sequence :
 *   -->set_fftw_wisdom(txt);
 *
 * Input : a scilab string matrix
 *
 * Output : Nothing or an error messsage if
 *          wisdom can't be read by fftw
 *
 */
int sci_set_fftw_wisdom __PARAMS((char *fname,unsigned long fname_len))
{
 int m1,n1,i,j,k,len;
 char **Str1;
 char *Str=NULL;

 CheckRhs(1,1);

 if (VarType(1)==10) {

  GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str1);

  len = 0; k = 0;
  for (j = 0; j < m1*n1; j++) {
    len += (int)strlen(Str1[j])+1;
    if ((Str = (char *)REALLOC(Str,sizeof(char)*(len))) == NULL) {
       Scierror(999,"%s: "
                    "Memory allocation error\n",
                    fname);
       return(0);
    }
    for (i = 0; i < (int)strlen(Str1[j]); i++) {
     Str[k+i] = Str1[j][i];
    }
    Str[k+strlen(Str1[j])] = '\n';
    k += (int)strlen(Str1[j])+1;
  }
  Str[k-1] = '\0';

  /*FreeRhsSVar(Str1);*/
  if (Str1) for (i = 0; i<m1*n1 ;i++) {
    if (Str1[i]) {
       FREE(Str1[i]);
       Str1[i]=NULL;
    }
  }
  if (Str1) {FREE(Str1); Str1=NULL;}

  if(!(call_fftw_import_wisdom_from_string(Str))) {
   Scierror(999,"%s: "
                "FFTW can't read wisdom\n",
                 fname);
  }
  FREE(Str);
 }
 else {
  Scierror(999,"%s: "
                "Bad Rhs type parameter\n",
                fname);
 }
 PutLhsVar();
 return(0);
}
/*-----------------------------------------------------------------------------------*/
