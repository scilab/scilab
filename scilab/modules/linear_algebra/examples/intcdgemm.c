#include "stack-c.h"

extern int C2F(dgemm) __PARAMS((char *,char *,integer *,integer *,integer *,
			    double *,double *,integer *,double *,integer *,
			    double *,double *,integer *));

int intcdgemm(fname)
     char* fname;
{ 
  int lalfa,m1,n1,m,n,k;
  int mA,nA,lA, mB,nB,lB,m4,n4,lbeta,mC,nC,lC;

  static int minlhs=1, minrhs=5, maxlhs=1, maxrhs=5;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  
  GetRhsVar(1,"d", &m1,&n1,&lalfa);   /*      alpha */
  GetRhsVar(2,"d", &mA,&nA,&lA);      /*      A     */
  GetRhsVar(3,"d", &mB,&nB,&lB);      /*      B     */
  GetRhsVar(4,"d", &m4,&n4,&lbeta);   /*      beta */
  GetRhsVar(5,"d", &mC,&nC,&lC);      /*      C     */
  m=mA;
  n=nB;
  if(nA != mB || m1*n1*m4*n4 != 1 )
    {
      sciprint("Bad call to dgemm\r\n");
      Error(9999);     return 0;
    }
  if(mA != mC || nB != nC )
    {
      sciprint("%s: invalid matrix dims in ",fname);
      Error(9999);     return 0;
    }
  k=nA;
  C2F(dgemm)("n","n",&m ,&n ,&k,stk(lalfa),
	stk(lA),&mA ,stk(lB),&mB ,stk(lbeta) ,stk(lC),&mC);
  /*      Return C (#5) */
  LhsVar(1) = 5;
  return(0);
}



