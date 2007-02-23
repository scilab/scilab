#include "stack-c.h"
/*      SUBROUTINE DLASSQ( N, X, INCX, SCALE, SUMSQ )     */
/*      [scale,sumsq]=dlassq(x,[ scale, [sumsq]])     */
int intdlassq(fname)
     char* fname;
{ 
  int ierr=0,un=1,deux=2,trois=3;
  int mX,nX,lX,N;
  int mSCALE,nSCALE,lSCALE,mSUMSQ,nSUMSQ,lSUMSQ;
  double SUMSQ, SCALE;
  int NRHS;

  static int minlhs=1, minrhs=1, maxlhs=2, maxrhs=3;
  CheckRhs(minrhs,maxrhs) ;  CheckLhs(minlhs,maxlhs) ;
  
  GetRhsVar(1, "d", &mX, &nX, &lX);   /* X */
  N=mX*nX;
  NRHS=Rhs;
  switch ( NRHS ) {
  case 3:
    /*    dlassq(x, scale, sumsq)   */
    GetRhsVar(2, "d", &mSCALE, &nSCALE, &lSCALE);   /* SCALE */
    GetRhsVar(3, "d", &mSUMSQ, &nSUMSQ, &lSUMSQ);   /* SUMSQ */
    break;
  case 2:
    /*    dlassq(x, scale)   */
    SUMSQ=0;
    GetRhsVar(2, "d", &mSCALE, &nSCALE, &lSCALE);   /* SCALE */
    CreateVar(3, "d", &un, &un, &lSUMSQ);
    *stk(lSUMSQ)=SUMSQ;   /*  default  */
    break;
  case 1:
    /*    dlassq(x)   */
    SUMSQ=0;SCALE=0;
    CreateVar(2, "d", &un, &un, &lSCALE);
    CreateVar(3, "d", &un, &un, &lSUMSQ);
    *stk(lSUMSQ)=SUMSQ; *stk(lSCALE)=SCALE;  /*  default  */
    break;
  default:
    sciprint("%s: bad call to dassq",fname);
    Error(9999); return 0;
  }
  C2F(dlassq)(&N , stk(lX), &un, stk(lSCALE), stk(lSUMSQ));
  LhsVar(1) = 2;  LhsVar(2) = 3; /*  Return var #2 (SCALE) and #3 (SUMSQ) */
  return(0);
}



