#include "stack-c.h"


/*--------------------------------------------------------
 * intview: interface for C displ function 
 *     should provide [X,Y,Z]=view(X,Y,Z)  at Scilab level 
 *
 *--------------------------------------------------------*/

extern void displ __PARAMS((double *a, int ma, int na, int *b, int mb, int nb, char *c, int mc, int nc));

int intview(fname) 
     char *fname;
{
  static int l1, m1, n1, l2, m2, n2, m3, n3, l3;
  static int minlhs=1, maxlhs=3, minrhs=3, maxrhs=3;

  /* Check number of inputs (rhs=3) and outputs (lhs=3) */
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* Get X (1 ,double), Y (2, int) and  C (3, string) */
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "i", &m2, &n2, &l2);
  GetRhsVar(3, "c", &m3, &n3, &l3);

  /* Call display function
     stk(l1)->X (double), istk(l2)->Y (int), cstk(l3)->Z  (char)    */
  displ(stk(l1), m1, n1, istk(l2), m2, n2, cstk(l3), m3, n3);

  /*  Return variables  */
  LhsVar(1) = 1;
  LhsVar(2) = 2;
  LhsVar(3) = 3;
  return 0;
}

/*--------------------------------------------------------
 * C function displ 
 *--------------------------------------------------------*/

void displ(a, ma, na, b, mb, nb, c, mc, nc)
     double *a; int *b; char *c;
     int ma, na, mb, nb, mc, nc;
{
  sciprint("First parameter (matrix) is %i x %i:\r\n",ma,na);
  sciprint("its (1,1) entry is %e (double).\r\n", a[0]);
  sciprint("Second parameter (matrix) is %i x %i:\r\n",mb,nb);
  sciprint("its (1,1) entry is %i (int).\r\n", b[0]);
  sciprint("Third parameter (string) is %i character long: it is the string \"%s\"\r\n",mc*nc, c);
}





