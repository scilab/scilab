#include "stack-c.h"


extern int C2F(dscal) __PARAMS((integer *n, double *da,double *dx,integer *incx));

/**************************************************
 * Calling the scilab function "mypoly" inside the interface 
 * "mypoly" is defined in ex12c.sce
 **************************************************/

int intex12c(fname) 
     char *fname;
{
  int mlhs,mrhs,ibegin,un=1,mn2;
  static int l1, m1, n1, m2, n2, l2 ; 
  static int minlhs=1, minrhs=2, maxlhs=1, maxrhs=2;
  static char name[] = "mypoly" ;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);

  /** multiply the roots by the scaling factor in place. */

  C2F(dscal)((mn2=m2*n2,&mn2),stk(l1),stk(l2),&un);

  /* call mypoly to create the polynomial from its roots 
   *
   *  To call a function it is required that its input arguments are
   *  stored in the last positions of the variables stack (it is the
   *  ase here. NOTE that when 
   *  called, the function destroys its input variables and replaces them by 
   *  the output variables. 
   *  Here  function  takes  variables 1 and 2 as inputs and generates output
   *  variables at positions 1.
   *  ibegin must be the index of the first input variable of a_function
   */
  ibegin=2;

  /* execute the function "mypoly" */
  mlhs=1; mrhs=1 ; 
  SciString(&ibegin,name,&mlhs,&mrhs);
  /* check if an error has occured while running mypoly */
  if (Err > 0 ) return 0;

  LhsVar(1) = 2; 
  return 0;
}

