#include "stack-c.h"

/**************************************************
 * Executing the Scilab function "myfct" defined in ex11c.sce
 **************************************************/

int intex11c(fname) 
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




