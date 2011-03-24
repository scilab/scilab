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
#include "gw_arnoldi.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(zneupd)(int * rvec, char * howmny, int * select, 
		       doublecomplex * d, doublecomplex * z, int * ldz, 
		       doublecomplex * sigma, doublecomplex * workev, 
		       char * bmat, int * n, char * which, int * nev, 
		       double *  tol, doublecomplex * resid, int * ncv,
		       doublecomplex * v, int * ldv, int * iparam, int * ipntr,
		       doublecomplex * workd, doublecomplex * workl, 
		       int * lworkl, double * rwork, int * info);
/*--------------------------------------------------------------------------*/
int sci_zneupd(char *fname,unsigned long fname_len)
{
  int mRVEC,     nRVEC,      pRVEC;
  int mHOWMANY,  nHOWMANY,   pHOWMANY;
  int mSELECT,   nSELECT,    pSELECT;
  int D,        mD,        nD,         pD;
  int Z,        mZ,        nZ,         pZ;
  int mSIGMA,    nSIGMA,     pSIGMA;
  int mWORKev,   nWORKev,    pWORKev;
  int mBMAT,     nBMAT,      pBMAT;
  int mN,        nN,         pN;
  int mWHICH,    nWHICH,     pWHICH;
  int mNEV,      nNEV,       pNEV;
  int mTOL,      nTOL,       pTOL;
  int RESID,    mRESID,    nRESID,     pRESID;
  int mNCV,      nNCV,       pNCV;
  int mV,        nV,         pV;
  int IPARAM,   mIPARAM,   nIPARAM,    pIPARAM;
  int IPNTR,    mIPNTR,    nIPNTR,     pIPNTR;
  int WORKD,    mWORKD,    nWORKD,     pWORKD;
  int WORKL,    mWORKL,    nWORKL,     pWORKL;
  int RWORK,    mRWORK,    nRWORK,     pRWORK;
  int INFO,     mINFO,     nINFO,      pINFO;

  int minlhs=1, minrhs=21, maxlhs=9, maxrhs=21;
  int LDZ, LDV, LWORKL;
  int sizeWORKL = 0;

  CheckRhs(minrhs,maxrhs);  
  CheckLhs(minlhs,maxlhs);

  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1,MATRIX_OF_INTEGER_DATATYPE, &mRVEC,   &nRVEC,   &pRVEC);
  GetRhsVar( 2,STRING_DATATYPE,            &mHOWMANY,&nHOWMANY,&pHOWMANY);
  GetRhsVar( 3,MATRIX_OF_INTEGER_DATATYPE, &mSELECT, &nSELECT, &pSELECT);
  GetRhsVar( 4,MATRIX_OF_COMPLEX_DATATYPE, &mD,      &nD,      &pD);                D =  4;
  GetRhsVar( 5,MATRIX_OF_COMPLEX_DATATYPE, &mZ,      &nZ,      &pZ);                Z =  5;
  GetRhsVar( 6,MATRIX_OF_COMPLEX_DATATYPE, &mSIGMA,  &nSIGMA,  &pSIGMA);
  GetRhsVar( 7,MATRIX_OF_COMPLEX_DATATYPE, &mWORKev, &nWORKev, &pWORKev);
  GetRhsVar( 8,STRING_DATATYPE,            &mBMAT,   &nBMAT,   &pBMAT);
  GetRhsVar( 9,MATRIX_OF_INTEGER_DATATYPE, &mN,      &nN,      &pN);
  GetRhsVar(10,STRING_DATATYPE,            &mWHICH,  &nWHICH,  &pWHICH);
  GetRhsVar(11,MATRIX_OF_INTEGER_DATATYPE, &mNEV,    &nNEV,    &pNEV);
  GetRhsVar(12,MATRIX_OF_DOUBLE_DATATYPE,  &mTOL,    &nTOL,    &pTOL);
  GetRhsVar(13,MATRIX_OF_COMPLEX_DATATYPE, &mRESID,  &nRESID,  &pRESID);        RESID = 13;
  GetRhsVar(14,MATRIX_OF_INTEGER_DATATYPE, &mNCV,    &nNCV,    &pNCV);
  GetRhsVar(15,MATRIX_OF_COMPLEX_DATATYPE, &mV,      &nV,      &pV);
  GetRhsVar(16,MATRIX_OF_INTEGER_DATATYPE, &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 16;
  GetRhsVar(17,MATRIX_OF_INTEGER_DATATYPE, &mIPNTR,  &nIPNTR,  &pIPNTR);        IPNTR = 17;
  GetRhsVar(18,MATRIX_OF_COMPLEX_DATATYPE, &mWORKD,  &nWORKD,  &pWORKD);        WORKD = 18;
  GetRhsVar(19,MATRIX_OF_COMPLEX_DATATYPE, &mWORKL,  &nWORKL,  &pWORKL);        WORKL = 19;
  GetRhsVar(20,MATRIX_OF_DOUBLE_DATATYPE,  &mRWORK,  &nRWORK,  &pRWORK);        RWORK = 20;
  GetRhsVar(21,MATRIX_OF_INTEGER_DATATYPE, &mINFO,   &nINFO,   &pINFO);          INFO = 21;

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

  if (mD*nD!=(*istk(pNEV)+1))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "D", *istk(pNEV) + 1);
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

  sizeWORKL = 3 * *istk(pNCV) * *istk(pNCV) + 5 * *istk(pNCV);

  if ((mWORKL*nWORKL<sizeWORKL))
    {
      Scierror(999,_("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
      return 0;
    }

  C2F(zneupd)(istk(pRVEC), cstk(pHOWMANY), istk(pSELECT), zstk(pD),
              zstk(pZ), &LDZ, zstk(pSIGMA), zstk(pWORKev),
              cstk(pBMAT), istk(pN), cstk(pWHICH), istk(pNEV),
              stk(pTOL), zstk(pRESID), istk(pNCV), zstk(pV),
              &LDV, istk(pIPARAM), istk(pIPNTR), zstk(pWORKD),
              zstk(pWORKL), &LWORKL, stk(pRWORK), istk(pINFO));

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("zneupd", istk(pINFO), 6L);
    return 0;
  }

  LhsVar(1) = D;
  LhsVar(2) = Z;
  LhsVar(3) = RESID;
  LhsVar(4) = IPARAM;
  LhsVar(5) = IPNTR;
  LhsVar(6) = WORKD; 
  LhsVar(7) = WORKL;
  LhsVar(8) = RWORK; 
  LhsVar(9) = INFO;

  PutLhsVar();

  return 0;
}
/*--------------------------------------------------------------------------*/
