#include "stack-c.h"

/* Creating a scilab variable from a pointer
 * l1 is a pointer to a double array created 
 * by the C function dblearrayc (in file pgmsc.c)   
 */

extern int dblearrayc __PARAMS((double **a, int *m, int *n,int *err));

int intex3c(fname)
  char* fname;
{ 
  double *l1;
  int m1,n1,ierr=0;
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  dblearrayc( &l1,&m1,&n1,&ierr);
  if ( ierr > 0 ) 
    {
      Scierror(999,"%s: Internal error \r\n",fname);
      return 0;
    }

  CreateVarFromPtr( 1, "d", &m1, &n1, &l1);
  FreePtr(&l1);
  LhsVar(1) = 1;
  return(0);
}
