#include "stack-c.h"

extern int intarrayc __PARAMS((int **a, int *m, int *n,int *err));

/*      Creating a scilab variable from a pointer
 *      l1 is a pointer to an int array created 
 *      by the C function intarrayc (in file pgmsc.c)   
 */

int intex4c(fname)
  char* fname;
{ 
  int *l1;

  int m1,n1,ierr=0;
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  intarrayc( &l1, &m1, &n1, &ierr);
  if ( ierr > 0 ) 
    {
      Scierror(999,"%s: Internal error \r\n",fname);
    }

  CreateVarFromPtr( 1, "i", &m1, &n1, &l1);
  FreePtr(&l1);
  LhsVar(1) = 1;
  return(0);
}
