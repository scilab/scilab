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
#include "doublecomplex.h"
/*--------------------------------------------------------------------------*/
#define CHAR(x)         (cstk(x))
#define INT(x)  	(istk(x))
#define DOUBLE(x)	( stk(x))
#define CMPLX(x)	(zstk(x))
/*--------------------------------------------------------------------------*/
extern int C2F(znaupd)();
/*--------------------------------------------------------------------------*/
int C2F(intznaupd)(char *fname,unsigned long fname_len)
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
  int RWORK, mRWORK, nRWORK,  pRWORK;
  int INFO,  mINFO,  nINFO,   pINFO;

  int minlhs=1, minrhs=15, maxlhs=9, maxrhs=15;
  int LDV, LWORKL;

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);

  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1,MATRIX_OF_INTEGER_DATATYPE, &mIDO,    &nIDO,    &pIDO);           IDO  =  1;
  GetRhsVar( 2,STRING_DATATYPE, &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  2;
  GetRhsVar( 3,MATRIX_OF_INTEGER_DATATYPE, &mN,      &nN,      &pN);             N    =  3;
  GetRhsVar( 4,STRING_DATATYPE, &mWHICH,  &nWHICH,  &pWHICH);       WHICH  =  4;
  GetRhsVar( 5,MATRIX_OF_INTEGER_DATATYPE, &mNEV,    &nNEV,    &pNEV);            NEV =  5;
  GetRhsVar( 6,MATRIX_OF_DOUBLE_DATATYPE, &mTOL,    &nTOL,    &pTOL);            TOL =  6;
  GetRhsVar( 7,MATRIX_OF_COMPLEX_DATATYPE, &mRESID,  &nRESID,  &pRESID);        RESID =  7;
  GetRhsVar( 8,MATRIX_OF_INTEGER_DATATYPE, &mNCV,    &nNCV,    &pNCV);            NCV =  8;
  GetRhsVar( 9,MATRIX_OF_COMPLEX_DATATYPE, &mV,      &nV,      &pV);               V  =  9;
  GetRhsVar(10,MATRIX_OF_INTEGER_DATATYPE, &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 10;
  GetRhsVar(11,MATRIX_OF_INTEGER_DATATYPE, &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 11;
  GetRhsVar(12,MATRIX_OF_COMPLEX_DATATYPE, &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 12;
  GetRhsVar(13,MATRIX_OF_COMPLEX_DATATYPE, &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 13;
  GetRhsVar(14,MATRIX_OF_DOUBLE_DATATYPE, &mRWORK,  &nRWORK,  &pRWORK);       RWORK  = 14;
  GetRhsVar(15,MATRIX_OF_INTEGER_DATATYPE, &mINFO,   &nINFO,   &pINFO);          INFO = 15;

  LWORKL = mWORKL*nWORKL;   LDV=Max(1,*istk(pN));

  C2F(znaupd)(istk(pIDO), cstk(pBMAT), istk(pN),
	      cstk(pWHICH), istk(pNEV), stk(pTOL),
              zstk(pRESID), istk(pNCV), zstk(pV), &LDV,
              istk(pIPARAM), istk(pIPNTR), zstk(pWORKD),
              zstk(pWORKL), &LWORKL, stk(pRWORK), istk(pINFO), 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("znaupd", istk(pINFO), 6L);
    return 0;
  }
  LhsVar(1)=IDO;    LhsVar(2)=RESID; LhsVar(3)=V;
  LhsVar(4)=IPARAM; LhsVar(5)=IPNTR;
  LhsVar(6)=WORKD;  LhsVar(7)=WORKL; LhsVar(8)=RWORK; LhsVar(9)=INFO;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
