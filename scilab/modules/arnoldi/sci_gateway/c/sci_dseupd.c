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
/*--------------------------------------------------------------------------*/
#define CHAR(x)         (cstk(x))
#define INT(x)  	(istk(x))
#define DOUBLE(x)	( stk(x))
#define CMPLX(x)	(zstk(x))
/*--------------------------------------------------------------------------*/
extern int C2F(dseupd)(int *rvec, char *howmny, int *select, double *d, double *z, int *ldz, double *sigma, char *bmat, int *n, char *which, int *nev ,double *tol, double *resid, int *ncv, double *v ,int *ldv, int *iparam,int *ipntr, double *workd,double *workl,int *lworkl,int *info, unsigned long rvec_length, unsigned long howmany_length, unsigned long bmat_length, unsigned long which_len);
/*--------------------------------------------------------------------------*/
int C2F(intdseupd)(char *fname,unsigned long fname_len)
{
  int RVEC,     mRVEC,     nRVEC,      pRVEC;
  int HOWMANY,  mHOWMANY,  nHOWMANY,   pHOWMANY;
  int SELECT,   mSELECT,   nSELECT,    pSELECT;
  int D,        mD,        nD,         pD;
  int Z,        mZ,        nZ,         pZ;
  int SIGMA,    mSIGMA,    nSIGMA,     pSIGMA;
  int BMAT,     mBMAT,     nBMAT,      pBMAT;
  int N,        mN,        nN,         pN;
  int WHICH,    mWHICH,    nWHICH,     pWHICH;
  int NEV,      mNEV,      nNEV,       pNEV;
  int TOL,      mTOL,      nTOL,       pTOL;
  int RESID,    mRESID,    nRESID,     pRESID;
  int NCV,      mNCV,      nNCV,       pNCV;
  int V,        mV,        nV,         pV;
  int IPARAM,   mIPARAM,   nIPARAM,    pIPARAM;
  int IPNTR,    mIPNTR,    nIPNTR,     pIPNTR;
  int WORKD,    mWORKD,    nWORKD,     pWORKD;
  int WORKL,    mWORKL,    nWORKL,     pWORKL;
  int INFO,     mINFO,     nINFO,      pINFO;

  int minlhs=1, minrhs=19, maxlhs=9, maxrhs=19;
  int LDZ, LDV, LWORKL;

  /* [D,Z,RESID,V,IPARAM,IPNTR,WORKD,WORKL,INFO]=dseupd...
     (RVEC,HOWMANY,SELECT,D,Z,SIGMA,BMAT,N,WHICH,NEV,TOL,RESID,NCV,V,IPARAM,IPNTR,WORKD,WORKL,INFO) */

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);
  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1,MATRIX_OF_INTEGER_DATATYPE, &mRVEC,   &nRVEC,   &pRVEC);         RVEC  =  1;
  GetRhsVar( 2,STRING_DATATYPE, &mHOWMANY,&nHOWMANY,&pHOWMANY);   HOWMANY  =  2;
  GetRhsVar( 3,MATRIX_OF_INTEGER_DATATYPE, &mSELECT, &nSELECT, &pSELECT);     SELECT  =  3;
  GetRhsVar( 4,MATRIX_OF_DOUBLE_DATATYPE, &mD,      &nD,      &pD);               D  =  4;
  GetRhsVar( 5,MATRIX_OF_DOUBLE_DATATYPE, &mZ,      &nZ,      &pZ);               Z  =  5;
  GetRhsVar( 6,MATRIX_OF_DOUBLE_DATATYPE, &mSIGMA,  &nSIGMA,  &pSIGMA);        SIGMA =  6;
  GetRhsVar( 7,STRING_DATATYPE, &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  7;
  GetRhsVar( 8,MATRIX_OF_INTEGER_DATATYPE, &mN,      &nN,      &pN);             N    =  8;
  GetRhsVar( 9,STRING_DATATYPE, &mWHICH,  &nWHICH,  &pWHICH);       WHICH  =  9;
  GetRhsVar(10,MATRIX_OF_INTEGER_DATATYPE, &mNEV,    &nNEV,    &pNEV);            NEV = 10;
  GetRhsVar(11,MATRIX_OF_DOUBLE_DATATYPE, &mTOL,    &nTOL,    &pTOL);            TOL = 11;
  GetRhsVar(12,MATRIX_OF_DOUBLE_DATATYPE, &mRESID,  &nRESID,  &pRESID);        RESID = 12;
  GetRhsVar(13,MATRIX_OF_INTEGER_DATATYPE, &mNCV,    &nNCV,    &pNCV);            NCV = 13;
  GetRhsVar(14,MATRIX_OF_DOUBLE_DATATYPE, &mV,      &nV,      &pV);               V  = 14;
  GetRhsVar(15,MATRIX_OF_INTEGER_DATATYPE, &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 15;
  GetRhsVar(16,MATRIX_OF_INTEGER_DATATYPE, &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 16;
  GetRhsVar(17,MATRIX_OF_DOUBLE_DATATYPE, &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 17;
  GetRhsVar(18,MATRIX_OF_DOUBLE_DATATYPE, &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 18;
  GetRhsVar(19,MATRIX_OF_INTEGER_DATATYPE, &mINFO,   &nINFO,   &pINFO);          INFO = 19;

  LWORKL = mWORKL*nWORKL;   LDV=Max(1,*istk(pN)); LDZ=LDV;

  C2F(dseupd)(istk(pRVEC), cstk(pHOWMANY),  istk(pSELECT),
	      stk(pD), stk(pZ),   &LDZ,
	      stk(pSIGMA), cstk(pBMAT), istk(pN), cstk(pWHICH),
              istk(pNEV), stk(pTOL), stk(pRESID),
              istk(pNCV), stk(pV), &LDV,
	      istk(pIPARAM), istk(pIPNTR),
               stk(pWORKD), stk(pWORKL), &LWORKL,
              istk(pINFO), 1L, 1L, 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("dseupd", istk(pINFO), 6L);
    return 0;
  }
  LhsVar(1)=D;    LhsVar(2)=Z;      LhsVar(3)=RESID;
  LhsVar(4)=V;    LhsVar(5)=IPARAM; LhsVar(6)=IPNTR;
  LhsVar(7)=WORKD;  LhsVar(8)=WORKL; LhsVar(9)=INFO;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/

