#include "stack-c.h"

/* Usage:  C=zgemm(alfa,A,B,beta,C)   */
/*    Ex:  alfa=1+%i;beta=alfa;A=rand(2,2)+%i*rand(2,2);B=A;C=A; */ 
           
int intzgemm(fname)
     char* fname;
{ 
  int lalfa,m1,n1,m,n,k;
  int mA,nA,lA, mB,nB,lB,m4,n4,lbeta,mC,nC,lC;

  int minlhs=1, minrhs=5, maxlhs=1, maxrhs=5;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  
  GetRhsVar(1,"z", &m1,&n1,&lalfa);   /*      alpha */
  CheckScalar(1, m1, n1);

  GetRhsVar(2,"z", &mA,&nA,&lA);      /*      A     */
  GetRhsVar(3,"z", &mB,&nB,&lB);      /*      B     */

  GetRhsVar(4,"z", &m4,&n4,&lbeta);   /*      beta */
  CheckScalar(4, m4, n4);

  GetRhsVar(5,"z", &mC,&nC,&lC);      /*      C     */

  m=mA;  n=nB;
  if(nA != mB || mA != mC || nB != nC ) Scierror(999,"%f: invalid matrix dims \r\n",fname);

  k=nA;
  C2F(zgemm)("n","n",&m ,&n ,&k,zstk(lalfa),
             zstk(lA),&mA ,zstk(lB),&mB ,zstk(lbeta) ,zstk(lC),&mC);
  /*      Return C (#5) */
  LhsVar(1) = 5;
  return(0);
}



