/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file is released under the 3-clause BSD license. See COPYING-BSD.
*/
/* -------------------------------------------------------------------------- */
/* WARNING:this gateway uses old scilab api and it does not work with Scilab 6*/
/* see help api_scilab for more information. */
/* -------------------------------------------------------------------------- */
#include "stack-c.h"
#include "Scierror.h"
/* -------------------------------------------------------------------------- */
/* Usage:  C = dgemm(alfa, A, B, betha, C) */
/* -------------------------------------------------------------------------- */
extern int C2F(dgemm)(char *,char *,int *,int *,int *,
                double *,double *,int *,double *,int *,
                double *,double *,int *);
/* -------------------------------------------------------------------------- */
int intdgemm(char* fname)
{
  int lalfa = 0, m1 = 0, n1 = 0, m = 0, n = 0, k = 0;
  int mA = 0, nA = 0, lA = 0, mB = 0, nB = 0, lB = 0, m4 = 0, n4 = 0, lbeta = 0;
  int mC = 0, nC = 0,lC = 0;

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

  m = mA;
  n = nB;
  if(nA != mB || mA != mC || nB != nC ) Scierror(999,"%f: invalid matrix dims\n",fname);

  k = nA;
  C2F(dgemm)("n","n",&m ,&n ,&k,stk(lalfa),
             stk(lA),&mA ,stk(lB),&mB ,stk(lbeta) ,stk(lC),&mC);
  /*      Return C (#5) */
  LhsVar(1) = 5;
  return(0);
}
/* -------------------------------------------------------------------------- */
