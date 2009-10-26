/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include <string.h>
#include "stack-c.h"
#include "core_math.h"
#include "gw_arnoldi.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dnaupd)(int *ido, char *bmat, int *n, char *which, int *nev, 
		       double *tol, double *resid, int *ncv, double *v, 
		       int *ldv, int *iparam, int *ipntr, double *workd, 
		       double *workl, int *lworkl, int *info, 
		       unsigned long bmat_len, unsigned long which_len);
/*--------------------------------------------------------------------------*/
int sci_dnaupd(char *fname,unsigned long fname_len)
{
  int IDO,   mIDO,   nIDO,    pIDO;
  int BMAT,  mBMAT,  nBMAT,   pBMAT;
  int N,     mN,     nN,      pN;
  int WHICH, mWHICH, nWHICH,  pWHICH;
  int NEV,   mNEV,   nNEV,    pNEV;
  int TOL,   mTOL,   nTOL,    pTOL;
  int RESID, mRESID, nRESID,  pRESID;
  int NCV,   mNCV,   nNCV,    pNCV;
  int V,     mV,     nV,      pV;
  int IPARAM,mIPARAM,nIPARAM, pIPARAM;
  int IPNTR, mIPNTR, nIPNTR,  pIPNTR;
  int WORKD, mWORKD, nWORKD,  pWORKD;
  int WORKL, mWORKL, nWORKL,  pWORKL;
  int INFO,  mINFO,  nINFO,   pINFO;

  int minlhs=1, minrhs=14, maxlhs=8, maxrhs=14;
  int LDV, LWORKL;
  int sizeWORKL = 0;

  /* [IDO,RESID,V,IPARAM,IPNTR,WORKD,WORKL,INFO]=dnaupd...
     (ID0,BMAT,N,WHICH,NEV,TOL,RESID,NCV,V,IPARAM,IPNTR,WORKD,WORKL,INFO) */

  CheckRhs(minrhs,maxrhs);  
  CheckLhs(minlhs,maxlhs);

  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1,MATRIX_OF_INTEGER_DATATYPE, &mIDO,    &nIDO,    &pIDO);            IDO =  1;
  GetRhsVar( 2,STRING_DATATYPE,            &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  2;
  GetRhsVar( 3,MATRIX_OF_INTEGER_DATATYPE, &mN,      &nN,      &pN);                N =  3;
  GetRhsVar( 4,STRING_DATATYPE,            &mWHICH,  &nWHICH,  &pWHICH);        WHICH =  4;
  GetRhsVar( 5,MATRIX_OF_INTEGER_DATATYPE, &mNEV,    &nNEV,    &pNEV);            NEV =  5;
  GetRhsVar( 6,MATRIX_OF_DOUBLE_DATATYPE,  &mTOL,    &nTOL,    &pTOL);            TOL =  6;
  GetRhsVar( 7,MATRIX_OF_DOUBLE_DATATYPE,  &mRESID,  &nRESID,  &pRESID);        RESID =  7;
  GetRhsVar( 8,MATRIX_OF_INTEGER_DATATYPE, &mNCV,    &nNCV,    &pNCV);            NCV =  8;
  GetRhsVar( 9,MATRIX_OF_DOUBLE_DATATYPE,  &mV,      &nV,      &pV);                V =  9;
  GetRhsVar(10,MATRIX_OF_INTEGER_DATATYPE, &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 10;
  GetRhsVar(11,MATRIX_OF_INTEGER_DATATYPE, &mIPNTR,  &nIPNTR,  &pIPNTR);        IPNTR = 11;
  GetRhsVar(12,MATRIX_OF_DOUBLE_DATATYPE,  &mWORKD,  &nWORKD,  &pWORKD);        WORKD = 12;
  GetRhsVar(13,MATRIX_OF_DOUBLE_DATATYPE,  &mWORKL,  &nWORKL,  &pWORKL);        WORKL = 13;
  GetRhsVar(14,MATRIX_OF_INTEGER_DATATYPE, &mINFO,   &nINFO,   &pINFO);          INFO = 14;

  LWORKL = mWORKL*nWORKL;   LDV=Max(1,*istk(pN));

  /* Don't call dnaupd if ido == 99 */
  if (*istk(pIDO)==99)
    {
      Scierror(999,_("%s: the computation is already terminated\n"),fname);
      return 0;
    }

  /* Check some sizes */
  if (mIPARAM*nIPARAM!=11)
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "IPARAM", 11);
      return 0;
    }

  if (mIPNTR*nIPNTR!=11)
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "IPNTR", 14);
      return 0;
    }

  if (mRESID*nRESID!=*istk(pN))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "RESID", *istk(pN));
      return 0;
    }
    
  if ((mV!=*istk(pN))&&(nV!=*istk(pNCV)))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"),fname, "V", *istk(pN),*istk(pNCV));
      return 0;
    }

  if (mWORKD*nWORKD<3 * *istk(pN))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "WORKD", 3* *istk(pN));
      return 0;
    }

  sizeWORKL = 3 * *istk(pNCV) * *istk(pNCV) + 6 * *istk(pNCV);

  if (mWORKL*nWORKL<sizeWORKL)
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "WORKL", sizeWORKL);
      return 0;
    }

  C2F(dnaupd)(istk(pIDO),   cstk(pBMAT),  istk(pN),
	      cstk(pWHICH), istk(pNEV),   stk(pTOL),
               stk(pRESID), istk(pNCV),   stk(pV), &LDV,
              istk(pIPARAM),istk(pIPNTR), stk(pWORKD),
               stk(pWORKL), &LWORKL,      istk(pINFO), 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("dnaupd", istk(pINFO), 6L);
    return 0;
  }

  LhsVar(1) = IDO;    
  LhsVar(2) = RESID; 
  LhsVar(3) = V;
  LhsVar(4) = IPARAM; 
  LhsVar(5) = IPNTR;
  LhsVar(6) = WORKD;  
  LhsVar(7) = WORKL; 
  LhsVar(8) = INFO;

  PutLhsVar();

  return 0;
}
/*--------------------------------------------------------------------------*/
