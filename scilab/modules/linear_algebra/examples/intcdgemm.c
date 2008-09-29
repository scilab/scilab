/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"

extern int C2F(dgemm)(char *,char *,int *,int *,int *,
			    double *,double *,int *,double *,int *,
			    double *,double *,int *);

int intcdgemm(fname)
     char* fname;
{ 
  int lalfa,m1,n1,m,n,k;
  int mA,nA,lA, mB,nB,lB,m4,n4,lbeta,mC,nC,lC;

  static int minlhs=1, minrhs=5, maxlhs=1, maxrhs=5;
  Nbvars = 0;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;
  
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1,&n1,&lalfa);   /*      alpha */
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &mA,&nA,&lA);      /*      A     */
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &mB,&nB,&lB);      /*      B     */
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &m4,&n4,&lbeta);   /*      beta */
  GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE, &mC,&nC,&lC);      /*      C     */
  m=mA;
  n=nB;
  if(nA != mB || m1*n1*m4*n4 != 1 )
    {
      sciprint("Bad call to dgemm\n");
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



