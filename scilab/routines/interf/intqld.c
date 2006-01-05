#include <string.h>
#include "../stack-c.h"
extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));

#ifdef __STDC__
extern int  Scierror __PARAMS((int iv,char *fmt,...));
#else
extern int Scierror __PARAMS(());
#endif 

int ql0001_(int *m,int *me,int *mmax,int *n,int *nmax,int *mnn,
            double *c,double *d,double *a,double *b,double *xl,
            double *xu,double *x,double *u,int *iout,int *ifail,
            int *iprint,double *war,int *lwar,int *iwar,int *liwar,
            double *eps1);

int C2F(intqld)(fname) 
     char *fname;
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

  /*    Define minls=1, maxlhs, minrhs, maxrhs   */
  static int minlhs=1, minrhs=7, maxlhs=3, maxrhs=8;

  /*   Check rhs and lhs   */  
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* RhsVar: qld(Q,p,C,b,lb,ub,me,eps) */
  /*             1,2,3,4,5 ,6 ,7, 8  */
  eps1=C2F(dlamch)("e",1L);
  next= Rhs+1;
  /*   Variable 1 (Q)   */
  GetRhsVar(1, "d", &n, &nbis, &Q);
  CheckSquare(1,n,nbis);

  /*   Variable 2 (p)   */
  GetRhsVar(2, "d", &nbis, &unbis, &p);
  CheckLength(2,nbis*unbis,n);
  
  /*   Variable 3 (C)   */
  GetRhsVar(3, "d", &m, &nbis, &C);
  if (( nbis != n ) && (m > 0)) 
    {
      Scierror(205,"qld: Argument 3: wrong number of columns %d expected \r\n", n);
      return 0;
    }
  mmax = m+1;
  mnn = m+n+n;

  /*   Variable 4 (b)   */  
  GetRhsVar(4, "d", &mbis, &unbis, &b);
  CheckLength(4,mbis*unbis,m);

  /*   Variable 5 (lb)   */
  GetRhsVar(5, "d", &nbis, &unbis, &lb);
  if (nbis*unbis==0) {
    CreateVar(next+1, "d", &n, &un, &lb);
    for(k=0; k<n; k++)
    stk(lb)[k] = -C2F(dlamch)("o",1L);
    next=next+1;
  }
  else
    CheckLength(5,nbis*unbis,n);


  /*   Variable 6 (ub)   */
  GetRhsVar(6, "d", &nbis, &unbis, &ub);
  if (nbis*unbis==0) {
    CreateVar(next+1, "d", &n, &un, &ub);
    for(k=0; k<n; k++)
    stk(ub)[k] = C2F(dlamch)("o",1L);
    next=next+1;
  }
  else
    CheckLength(6,nbis*unbis,n);
 
  /*   Variable 7 (me)   */  
  GetRhsVar(7, "i", &pipo, &unbis, &me);
  CheckScalar(7,pipo,unbis);
  if ((*istk(me)<0) || (*istk(me)>n))
    {
      Err = 7;
      Error(116);
      return 0;
    }

  if(Rhs==8) {
    /*   Variable 8 (eps1)   */  
    GetRhsVar(8, "d", &pipo, &unbis, &leps);
    CheckScalar(8,pipo,unbis);
    eps1= Max(eps1,*stk(leps));
  }

  next=Rhs;
  /* Internal variables: x, lambda, inform, C_mmax, b_mmax */
  CreateVar(next+1, "d", &n, &un, &x);

  CreateVar(next+2, "d", &mnn, &un, &lambda);
 
  CreateVar(next+3, "i", &un, &un, &inform);
  

  lwar = 3*n*n/2+10*n+2*mmax+2;
  CreateVar(next+4, "d", &lwar, &un, &war);
  CreateVar(next+5, "i", &n, &un, &iwar);
  istk(iwar)[0]=0;
 
  /* extend C and B to add a row and change the sign of C*/
  CreateVar(next+6, "d", &mmax, &n, &C_mmax);
  for(k=0; k<n; k++) {
    for(l=0; l<m; l++)
      stk(C_mmax)[k*mmax+l] = -stk(C)[k*m+l];
    stk(C_mmax)[k*mmax+m] = 0.0;}

  CreateVar(next+7, "d", &mmax, &un, &b_mmax);
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
    Scierror(24,"qld: Too many iterations (more than %d) \r\n", 40*(n+m));
  else if (ifail==2)
    Scierror(24,"qld: Accuracy insufficient to statify convergence criterion.\r\n");
  else if (ifail==5)
    Scierror(999,"qld: Length of working array is too short.\r\n");
 else if (ifail>10)
    Scierror(999,"qld: The constraints are inconsistent.\r\n");


  return 0;
}
