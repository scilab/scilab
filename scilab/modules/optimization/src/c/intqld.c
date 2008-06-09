#include "intqld.h"
#include "localization.h"


int C2F(intqld)(char *fname)
{
  static int un=1,zero=0;

  static int Q, n, nbis;
  static int p, unbis;
  static int C, mmax, m, mnn;
  static int b, mbis;
  static int me, pipo;
  static int lb, ub, leps;
  static int x, lambda, inform, ifail;
  int next;

  static int war, lwar, iwar, iout, C_mmax, b_mmax, k, l;
  static double eps1;

  /*   Check rhs and lhs   */
  CheckRhs(7,8) ;
  CheckLhs(1,3) ;

  /* RhsVar: qld(Q,p,C,b,lb,ub,me,eps) */
  /*             1,2,3,4,5 ,6 ,7, 8  */
  eps1=C2F(dlamch)("e",1L);
  next= Rhs+1;
  /*   Variable 1 (Q)   */
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &n, &nbis, &Q);
  CheckSquare(1,n,nbis);

  /*   Variable 2 (p)   */
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &p);
  CheckLength(2,nbis*unbis,n);

  /*   Variable 3 (C)   */
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &m, &nbis, &C);
  if (( nbis != n ) && (m > 0))
    {
      Scierror(205,_("Wrong size for input argument #%d: number of columns %d expected.\n"),"qld",3,n);
      return 0;
    }
  mmax = m+1;
  mnn = m+n+n;

  /*   Variable 4 (b)   */
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &mbis, &unbis, &b);
  CheckLength(4,mbis*unbis,m);

  /*   Variable 5 (lb)   */
  GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &lb);
  if (nbis*unbis==0) {
    CreateVar(next,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &lb);
    for(k=0; k<n; k++)
    stk(lb)[k] = -C2F(dlamch)("o",1L);
    next=next+1;
  }
  else
    CheckLength(5,nbis*unbis,n);


  /*   Variable 6 (ub)   */
  GetRhsVar(6,MATRIX_OF_DOUBLE_DATATYPE, &nbis, &unbis, &ub);
  if (nbis*unbis==0) {
    CreateVar(next,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &ub);
    for(k=0; k<n; k++)
    stk(ub)[k] = C2F(dlamch)("o",1L);
    next=next+1;
  }
  else
    CheckLength(6,nbis*unbis,n);

  /*   Variable 7 (me)   */
  GetRhsVar(7,MATRIX_OF_INTEGER_DATATYPE, &pipo, &unbis, &me);
  CheckScalar(7,pipo,unbis);
  if ((*istk(me)<0) || (*istk(me)>n))
    {
      Err = 7;
      Error(116);
      return 0;
    }

  if(Rhs==8) {
    /*   Variable 8 (eps1)   */
    GetRhsVar(8,MATRIX_OF_DOUBLE_DATATYPE, &pipo, &unbis, &leps);
    CheckScalar(8,pipo,unbis);
    eps1= Max(eps1,*stk(leps));
  }

  /* Internal variables: x, lambda, inform, C_mmax, b_mmax */
  CreateVar(next+1,MATRIX_OF_DOUBLE_DATATYPE, &n, &un, &x);

  CreateVar(next+2,MATRIX_OF_DOUBLE_DATATYPE, &mnn, &un, &lambda);

  CreateVar(next+3,MATRIX_OF_INTEGER_DATATYPE, &un, &un, &inform);


  lwar = 3*n*n/2+10*n+2*mmax+2;
  CreateVar(next+4,MATRIX_OF_DOUBLE_DATATYPE, &lwar, &un, &war);
  CreateVar(next+5,MATRIX_OF_INTEGER_DATATYPE, &n, &un, &iwar);
  istk(iwar)[0]=1;/*Cholesky fcatorisation required*/

  /* extend C and B to add a row and change the sign of C*/
  CreateVar(next+6,MATRIX_OF_DOUBLE_DATATYPE, &mmax, &n, &C_mmax);
  for(k=0; k<n; k++) {
    for(l=0; l<m; l++)
      stk(C_mmax)[k*mmax+l] = -stk(C)[k*m+l];
    stk(C_mmax)[k*mmax+m] = 0.0;}

  CreateVar(next+7,MATRIX_OF_DOUBLE_DATATYPE, &mmax, &un, &b_mmax);
  for(k=0; k<m; k++)
    stk(b_mmax)[k] = stk(b)[k];
  stk(b_mmax)[m] = 0.0;

  iout = 0;
  ifail=0;
  C2F(ql0001)(&m, istk(me), &mmax, &n, &n, &mnn, stk(Q), stk(p), stk(C_mmax),
          stk(b_mmax), stk(lb), stk(ub), stk(x), stk(lambda), &iout,
          &ifail, &zero, stk(war), &lwar, istk(iwar), &n, &eps1);


  /* LhsVar: [x, lambda, inform] = qld(...) */

  if (ifail==0) {
    LhsVar(1) = next+1;
    LhsVar(2) = next+2;
    if (Lhs==3) {
      LhsVar(3) = next+3;
      *istk(inform)=ifail;
    }
    C2F(putlhsvar)();
  }
  else if (ifail==1)
    Scierror(24,_("%s: Too many iterations (more than %d).\n"),"qld",40*(n+m));
  else if (ifail==2)
    Scierror(24,_("%s: Accuracy insufficient to statify convergence criterion.\n"),"qld");
  else if (ifail==5)
    Scierror(999,_("%s: Length of working array is too short.\n"),"qld");
 else if (ifail>10)
    Scierror(999,_("%s: The constraints are inconsistent.\n"),"qld");


  return 0;
}
