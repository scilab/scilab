#include "stack-c.h"
/*      SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT,
       $                   WORK, LWORK, INFO)   */
 
/*     [U,s,VT]=svd(A)      */
/*     [U,s]=svd(A)         */
/*       s = svd(A)         */

#define MAX(x,y)	(((x)>(y))?(x):(y))
#define MIN(x,y)	(((x)<(y))?(x):(y))

int intdgesvd(fname)
     char* fname;
{ 
  int M,N,lA;
  int NRHS, NLHS;
  int mU, nU, lU, U;
  int mVT, nVT, lVT, VT;
  int ls, ns, s;
  int un=1; int WORK, LWORKMIN, LWORK, lw, INFO;
  int LDA, LDU, LDVT;

  CheckRhs(1,1) ;  CheckLhs(3,3) ;

  /*--------------------A---------------------------*/
  GetRhsVar(1, "d", &M, &N, &lA);

  NRHS=Rhs; NLHS=Lhs;
  switch ( NLHS ) {
  case 3:        /*  [U,s,VT]=svd(A)     */
  /*--------------------U---------------------------*/
    mU=M; nU=M;
    U=NRHS+1;
    CreateVar(U, "d", &mU, &nU, &lU);   /* U */
  /*--------------------s---------------------------*/
    ns=M; if (M>N) ns=N;
    s=NRHS+2;
    CreateVar(s, "d", &un, &ns, &ls);    /* s  */
  /*--------------------VT---------------------------*/
    mVT=N; nVT=N;
    VT=NRHS+3;
    CreateVar(VT, "d", &mVT, &nVT, &lVT);   /* VT */
  /*--------------------WORK---------------------------*/
    WORK=NRHS+4;
    LWORK=Maxvol(WORK, "d");  /*  max memory currently available */
    LWORKMIN = MAX(3*MIN(M,N)+MAX(M,N),5*MIN(M,N)-4);
    if (LWORK < LWORKMIN) { sciprint("not enough memory (use stacksize)");
       Error(9999); };
    CreateVar(NRHS+4, "d", &LWORK, &un, &lw);   /* WORK */
    LDA=M; LDU=M; LDVT=N;

    C2F(dgesvd)("A", "A", &M, &N, stk(lA), &LDA, stk(ls), stk(lU), &LDU, stk(lVT), &LDVT, stk(lw), &LWORK, &INFO);

    LhsVar(1)=U; LhsVar(2)=s; LhsVar(3)=VT;
    break;
  case 2:
    /*   [U,s]=svd(A)  To be done  */
    break;
  case 1:
    /*    s=svd(A) To be done  */
    break;
  default:
    return 0;
 }
  return(0);
}



