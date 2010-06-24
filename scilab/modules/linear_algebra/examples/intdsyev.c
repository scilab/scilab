#include "stack-c.h"
#include "core_math.h"
/*      SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO ) */
/*     [w,U]=dsyev(A)      */
/*     w=dsyev(A)      */

int intdsyev(fname)
     char* fname;
{ 
  int M,N,lA;
  int NLHS;
  int mU, nU, lU, U;
  int un=1; int WORK, lWORK, LWORKMIN, LWORK, INFO;
  int LDA, lw;
  int A=1, W=2;

  static int minlhs=1, minrhs=1, maxlhs=2, maxrhs=1;
  CheckRhs(minrhs,maxrhs) ;  CheckLhs(minlhs,maxlhs) ;

  /*--------------------A---------------------------*/
  GetRhsVar(A,MATRIX_OF_DOUBLE_DATATYPE, &M, &N, &lA);
  /*  To be done: Check A symmetric and Real*/
  NLHS=Lhs;
  switch ( NLHS ) {
  case 2:        
    /*  [w,U]=dsyev(A)     */
    CreateVar(W,MATRIX_OF_DOUBLE_DATATYPE, &N, &un, &lw);
    WORK=3;
    LWORKMIN = MAX(1, 3*N-1);
    LWORK = LWORKMIN;
    /* LWORK=C2F(maxvol)(&WORK, "d", 1L);   max memory currently available */
    if (LWORK < LWORKMIN) { 
      sciprint("not enough memory (use stacksize)");
      SciError(9999); };
    CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &LWORK, &un, &lWORK);
    LDA = MAX(1,N);
    C2F(dsyev)("V", "L", &N, stk(lA), &LDA, stk(lw), stk(lWORK), &LWORK, &INFO);
    if (INFO != 0) C2F(errorinfo)("dsyev ", &INFO, 5L);
    LhsVar(1)=A; LhsVar(2)=W;
    return 0;
    break;
  case 1:
    /*   w=dsyev(A)    */
    CreateVar(W,MATRIX_OF_DOUBLE_DATATYPE, &N, &un, &lw);
    WORK=3;
    LWORKMIN = MAX(1, 3*N-1);
    LWORK=C2F(maxvol)(&WORK, "d", 1L);  /* max memory currently available */
    if (LWORK < LWORKMIN) { 
      sciprint("not enough memory (use stacksize)");
      SciError(9999); };
    CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE, &LWORK, &un, &lWORK);
    LDA = MAX(1,N);
    C2F(dsyev)("N", "L", &N, stk(lA), &LDA, stk(lw), stk(lWORK), &LWORK, &INFO);
    if (INFO != 0) C2F(errorinfo)("dsyev ", &INFO, 5L);
    LhsVar(1)=W;
    return 0;
    break;
  default:
    return 0;
  }
  return(0);
}



