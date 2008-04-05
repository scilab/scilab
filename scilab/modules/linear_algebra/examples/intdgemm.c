#include "stack-c.h"

/* Usage:  C=dgemm(alfa,A,B,beta,C)   */

extern int C2F(dgemm)(char *,char *,integer *,integer *,integer *,
			    double *,double *,integer *,double *,integer *,
			    double *,double *,integer *);

int intdgemm(fname)
     char* fname;
{ 
  int lalfa,m1,n1,m,n,k;
  int mA,nA,lA, mB,nB,lB,m4,n4,lbeta,mC,nC,lC;

  int minlhs=1, minrhs=5, maxlhs=1, maxrhs=5;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1,&n1,&lalfa);   /*      alpha */
  CheckScalar(1, m1, n1);

  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &mA,&nA,&lA);      /*      A     */
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &mB,&nB,&lB);      /*      B     */

  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &m4,&n4,&lbeta);   /*      beta */
  CheckScalar(4, m4, n4);

  GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE, &mC,&nC,&lC);      /*      C     */

  m=mA;  n=nB;
  if(nA != mB || mA != mC || nB != nC ) Scierror(999,"%f: invalid matrix dims\n",fname);

  k=nA;
  C2F(dgemm)("n","n",&m ,&n ,&k,stk(lalfa),
             stk(lA),&mA ,stk(lB),&mB ,stk(lbeta) ,stk(lC),&mC);
  /*      Return C (#5) */
  LhsVar(1) = 5;
  return(0);
}



