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
extern int C2F(dneupd)(int *rvec, char *howmny, int *select, double *dr, 
		       double *di, double *z, int *ldz, double *sigmar, 
		       double *sigmai, double *workev, char *bmat, int *n, 
		       char *which, int *nev, double *tol, double *resid, 
		       int *ncv, double *v, int *ldv, int *iparam, int *ipntr, 
		       double *workd, double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/
int sci_dneupd(char *fname,unsigned long fname_len)
{
  int mRVEC,     nRVEC,      pRVEC;
  int mHOWMANY,  nHOWMANY,   pHOWMANY;
  int mSELECT,   nSELECT,    pSELECT;
  int Dr,       mDr,       nDr,        pDr;
  int Di,       mDi,       nDi,        pDi;
  int Z,        mZ,        nZ,         pZ;
  int mSIGMAr,   nSIGMAr,    pSIGMAr;
  int mSIGMAi,   nSIGMAi,    pSIGMAi;
  int mWORKev,   nWORKev,    pWORKev;
  int mBMAT,     nBMAT,      pBMAT;
  int mN,        nN,         pN;
  int mWHICH,    nWHICH,     pWHICH;
  int mNEV,      nNEV,       pNEV;
  int mTOL,      nTOL,       pTOL;
  int RESID,    mRESID,    nRESID,     pRESID;
  int mNCV,      nNCV,       pNCV;
  int V,        mV,        nV,         pV;
  int IPARAM,   mIPARAM,   nIPARAM,    pIPARAM;
  int IPNTR,    mIPNTR,    nIPNTR,     pIPNTR;
  int WORKD,    mWORKD,    nWORKD,     pWORKD;
  int WORKL,    mWORKL,    nWORKL,     pWORKL;
  int INFO,     mINFO,     nINFO,      pINFO;

  int minlhs=1, minrhs=22, maxlhs=10, maxrhs=22;
  int LDZ, LDV, LWORKL;
  int sizeWORKL = 0;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1,MATRIX_OF_INTEGER_DATATYPE, &mRVEC,   &nRVEC,   &pRVEC);
  GetRhsVar( 2,STRING_DATATYPE,            &mHOWMANY,&nHOWMANY,&pHOWMANY);
  GetRhsVar( 3,MATRIX_OF_INTEGER_DATATYPE, &mSELECT, &nSELECT, &pSELECT);
  GetRhsVar( 4,MATRIX_OF_DOUBLE_DATATYPE,  &mDr,     &nDr,     &pDr);              Dr =  4;
  GetRhsVar( 5,MATRIX_OF_DOUBLE_DATATYPE,  &mDi,     &nDi,     &pDi);              Di =  5;
  GetRhsVar( 6,MATRIX_OF_DOUBLE_DATATYPE,  &mZ,      &nZ,      &pZ);                Z =  6;
  GetRhsVar( 7,MATRIX_OF_DOUBLE_DATATYPE,  &mSIGMAr, &nSIGMAr, &pSIGMAr);
  GetRhsVar( 8,MATRIX_OF_DOUBLE_DATATYPE,  &mSIGMAi, &nSIGMAi, &pSIGMAi);
  GetRhsVar( 9,MATRIX_OF_DOUBLE_DATATYPE,  &mWORKev, &nWORKev, &pWORKev);
  GetRhsVar(10,STRING_DATATYPE,            &mBMAT,   &nBMAT,   &pBMAT);
  GetRhsVar(11,MATRIX_OF_INTEGER_DATATYPE, &mN,      &nN,      &pN);
  GetRhsVar(12,STRING_DATATYPE,            &mWHICH,  &nWHICH,  &pWHICH);
  GetRhsVar(13,MATRIX_OF_INTEGER_DATATYPE, &mNEV,    &nNEV,    &pNEV);
  GetRhsVar(14,MATRIX_OF_DOUBLE_DATATYPE,  &mTOL,    &nTOL,    &pTOL);
  GetRhsVar(15,MATRIX_OF_DOUBLE_DATATYPE,  &mRESID,  &nRESID,  &pRESID);        RESID = 15;
  GetRhsVar(16,MATRIX_OF_INTEGER_DATATYPE, &mNCV,    &nNCV,    &pNCV);
  GetRhsVar(17,MATRIX_OF_DOUBLE_DATATYPE,  &mV,      &nV,      &pV);                V = 17;
  GetRhsVar(18,MATRIX_OF_INTEGER_DATATYPE, &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 18;
  GetRhsVar(19,MATRIX_OF_INTEGER_DATATYPE, &mIPNTR,  &nIPNTR,  &pIPNTR);        IPNTR = 19;
  GetRhsVar(20,MATRIX_OF_DOUBLE_DATATYPE,  &mWORKD,  &nWORKD,  &pWORKD);        WORKD = 20;
  GetRhsVar(21,MATRIX_OF_DOUBLE_DATATYPE,  &mWORKL,  &nWORKL,  &pWORKL);        WORKL = 21;
  GetRhsVar(22,MATRIX_OF_INTEGER_DATATYPE, &mINFO,   &nINFO,   &pINFO);          INFO = 22;

  LWORKL = mWORKL*nWORKL;   LDV=Max(1,*istk(pN)); LDZ=LDV;

  /* Check some sizes */
  if (mIPARAM*nIPARAM!=11)
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "IPARAM", 11);
      return 0;
    }

  if (mIPNTR*nIPNTR!=14)
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "IPNTR", 14);
      return 0;
    }

  if (mRESID*nRESID!=*istk(pN))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "RESID", *istk(pN));
      return 0;
    }

  if (mWORKD*nWORKD<3 * *istk(pN))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"),fname, "WORKD", 3* *istk(pN));
      return 0;
    }

  if (mSELECT*nSELECT!=*istk(pNCV))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "SELECT", *istk(pNCV));
      return 0;
    }

  if (mDr*nDr!=(*istk(pNEV)+1))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "Dr", *istk(pNEV) + 1);
      return 0;
    }

  if (mDi*nDi!=(*istk(pNEV)+1))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "Di", *istk(pNEV) + 1);
      return 0;
    }

  if ((mZ!=*istk(pN))&&(nZ!=*istk(pNEV)+1))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "Z", *istk(pN), *istk(pNEV) + 1);
      return 0;
    }

  if (mWORKev*nWORKev!=3 * *istk(pNCV))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKev", 3 * *istk(pNCV));
      return 0;
    }

  if ((mV!=*istk(pN))&&(mV!=*istk(pNCV)))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "V", *istk(pN),*istk(pNCV));
      return 0;
    }

  sizeWORKL = 3 * *istk(pNCV) * *istk(pNCV) + 6 * *istk(pNCV);

  if ((mWORKL*nWORKL<sizeWORKL))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
      return 0;
    }


  C2F(dneupd)(istk(pRVEC), cstk(pHOWMANY),  istk(pSELECT),
	      stk(pDr), stk(pDi), stk(pZ),   &LDZ,
	      stk(pSIGMAr), stk(pSIGMAi), stk(pWORKev),
              cstk(pBMAT), istk(pN), cstk(pWHICH),
              istk(pNEV), stk(pTOL), stk(pRESID),
              istk(pNCV), stk(pV), &LDV,
	      istk(pIPARAM), istk(pIPNTR),
               stk(pWORKD), stk(pWORKL), &LWORKL,
              istk(pINFO));

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("dneupd", istk(pINFO), 6L);
    return 0;
  }

  LhsVar(1)  = Dr;
  LhsVar(2)  = Di;
  LhsVar(3)  = Z;
  LhsVar(4)  = RESID; 
  LhsVar(5)  = V; 
  LhsVar(6)  = IPARAM;
  LhsVar(7)  = IPNTR;  
  LhsVar(8)  = WORKD; 
  LhsVar(9)  = WORKL; 
  LhsVar(10) = INFO;

  PutLhsVar();

  return 0;
}
/*--------------------------------------------------------------------------*/
