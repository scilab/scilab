#include "stack-c.h"

/**************************************************
 * An example of an hand written interface 
 * We call a scilab function (disp) inside the interface 
 **************************************************/

int intex13c_1(fname) 
     char *fname;
{
  int mlhs,mrhs,ibegin;
  static int l1, m1, n1, m2, n2, l2 ; 
  static int minlhs=1, minrhs=2, maxlhs=1, maxrhs=2;
  static char name[] = "disp" ;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);

  /* 
   *  To call a function it is required that its input arguments are
   *  stored in the last positions of the variables stack (it is the
   *  case here. NOTE that when 
   *  called, the function destroys its input variables and replaces them by 
   *  the output variables. 
   *  Here  function  takes  variables 1 and 2 as inputs and generates output
   *  variables at positions 1.
   *  ibegin must be the index of the first input variable of a_function
   */
  ibegin=1;

  /* execute the Scilab disp function */
  mlhs=1; mrhs=2; 
  SciString(&ibegin,name,&mlhs,&mrhs);
  /* check if an error has occured while running a_function */
  if (Err > 0 ) return 0;

  /*  Note that disp, as every function which has nothing to return,
   *  creates as output a variable with special type 0.
   */

  LhsVar(1) = 0; 
  return 0;
}


/**************************************************
 * Executing the Scilab function "myfct" defined in ex11c.sce
 **************************************************/

int intex13c_2(fname) 
     char *fname;
{
  int mlhs,mrhs,ibegin;
  static int l1, m1, n1, m2, n2, l2 ;
  static int minlhs=1, minrhs=6, maxlhs=3, maxrhs=6;
  static char name[] = "myfct" ;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m2, &n2, &l2);
  GetRhsVar(4, "d", &m2, &n2, &l2);
  GetRhsVar(5, "d", &m2, &n2, &l2);
  GetRhsVar(6, "d", &m2, &n2, &l2);

  /* 
   *  We receive 6 input variables indexed by (1,2,...6) 
   *  when the command ex11c(x1,x2,x3,x4,x5,x6) is issued.
   *  We have a Scilab function "myfct" with mrhs=2 inputs 
   *  and mlhs=3 outputs:
   *  function [u,v,w]=myfct(x,y)','u=7+x,v=8+y,w=9+y')
   *  To run myfct with input variables x5 and x6,
   *  we must set ibegin=5. Variables passed to the function must
   *  appear consecutively with index ifirst, ifirst+1,..., ifirst+mrhs. 
   */
  ibegin=5;  mlhs=3; mrhs=2 ; 
  /* execute the function "myfct" */
  SciString(&ibegin,name,&mlhs,&mrhs);

  /* check if an error has occured while running a_function */
  if (Err > 0 ) return 0;
  
  /* Output variables u, v, and w of myfct
   * are now indexed by ifirst, ifirst+1, ifirst+mlhs i.e.
   * u and v are indexed by 5 and 6 resp. and w (created by myfct)
   * is indexed by 7.
   * We return u v and w:
   * Caution: Variables with index larger than ifirst+mrhs cannot be 
   *  returned to Scilab.
   */

  LhsVar(1) = 5;
  LhsVar(2) = 6;
  LhsVar(3) = 7;
  return 0;
}



extern int C2F(dscal) __PARAMS((integer *n, double *da,double *dx,integer *incx));

/**************************************************
 * Calling the scilab function "mypoly" inside the interface 
 * "mypoly" is defined in ex12c.sce
 **************************************************/

int intex13c_3(fname) 
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







