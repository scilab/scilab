#include "stack-c.h"
/*  SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )  */
/*      [A,IPIV,INFO]=dgetrf(A)     */
int intdgetrf(fname)
     char* fname;
{ 
  int M,N,lA,LDA;
  int INFO, Nipiv, lIPIV, lINFO, un=1;
  static int minlhs=1, minrhs=1, maxlhs=3, maxrhs=1;
  CheckRhs(minrhs,maxrhs) ;  CheckLhs(minlhs,maxlhs) ;
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &M, &N, &lA);   /* A */
  LDA=M;  if (M==0) LDA=1;
  Nipiv=N; if (M<N) Nipiv=M;
  CreateVar(2,MATRIX_OF_INTEGER_DATATYPE, &Nipiv, &un, &lIPIV);   /*  IPIV  */
  CreateVar(3,MATRIX_OF_INTEGER_DATATYPE, &un, &un, &lINFO);   /*  IPIV  */
  C2F(dgetrf)(&M , &N, stk(lA), &LDA, istk(lIPIV), istk(lINFO));
  LhsVar(1) = 1;  LhsVar(2) = 2; /*  Return var #1 (A) and #3 (IPIV) */
  LhsVar(3) = 3;  /*   INFO  */
  return(0);
}



