#include <math.h>
#include <string.h>
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
#define MAX(x,y)	(((x)>(y))?(x):(y))
#define CHAR(x)         (cstk(x))
#define INT(x)  	(istk(x))
#define DOUBLE(x)	( stk(x))
#define CMPLX(x)	(zstk(x))
/*-----------------------------------------------------------------------------------*/
extern int C2F(dneupd) __PARAMS((int *rvec, char *howmny, int *select, double *dr, double *di, double *z, int *ldz, double *sigmar, double *sigmai, double *workev, char *bmat, int *n, char *which, int *nev, double *tol, double *resid, int *ncv, double *v, int *ldv, int *iparam, int *ipntr, double *workd, double *workl, int *lworkl, int *info, unsigned long howmany_length, unsigned long bmat_length, unsigned long which_length));
/*-----------------------------------------------------------------------------------*/
int C2F(intdneupd) _PARAMS((char *fname,unsigned long fname_len))
{ 
  int RVEC,     mRVEC,     nRVEC,      pRVEC;
  int HOWMANY,  mHOWMANY,  nHOWMANY,   pHOWMANY;
  int SELECT,   mSELECT,   nSELECT,    pSELECT;
  int Dr,       mDr,       nDr,        pDr;
  int Di,       mDi,       nDi,        pDi;
  int Z,        mZ,        nZ,         pZ;
  int SIGMAr,   mSIGMAr,   nSIGMAr,    pSIGMAr;
  int SIGMAi,   mSIGMAi,   nSIGMAi,    pSIGMAi;
  int WORKev,   mWORKev,   nWORKev,    pWORKev;
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

  int minlhs=1, minrhs=22, maxlhs=10, maxrhs=22;
  int LDZ, LDV, LWORKL;

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);
  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1, "i", &mRVEC,   &nRVEC,   &pRVEC);         RVEC  =  1;
  GetRhsVar( 2, "c", &mHOWMANY,&nHOWMANY,&pHOWMANY);   HOWMANY  =  2;
  GetRhsVar( 3, "i", &mSELECT, &nSELECT, &pSELECT);     SELECT  =  3;
  GetRhsVar( 4, "d", &mDr,     &nDr,     &pDr);             Dr  =  4;
  GetRhsVar( 5, "d", &mDi,     &nDi,     &pDi);             Di  =  5;
  GetRhsVar( 6, "d", &mZ,      &nZ,      &pZ);               Z  =  6;
  GetRhsVar( 7, "d", &mSIGMAr, &nSIGMAr, &pSIGMAr);      SIGMAr =  7;
  GetRhsVar( 8, "d", &mSIGMAi, &nSIGMAi, &pSIGMAi);      SIGMAi =  8;
  GetRhsVar( 9, "d", &mWORKev, &nWORKev, &pWORKev);      WORKev =  9;
  GetRhsVar(10, "c", &mBMAT,   &nBMAT,   &pBMAT);          BMAT = 10;
  GetRhsVar(11, "i", &mN,      &nN,      &pN);             N    = 11;  
  GetRhsVar(12, "c", &mWHICH,  &nWHICH,  &pWHICH);       WHICH  = 12; 
  GetRhsVar(13, "i", &mNEV,    &nNEV,    &pNEV);            NEV = 13;
  GetRhsVar(14, "d", &mTOL,    &nTOL,    &pTOL);            TOL = 14; 
  GetRhsVar(15, "d", &mRESID,  &nRESID,  &pRESID);        RESID = 15;
  GetRhsVar(16, "i", &mNCV,    &nNCV,    &pNCV);            NCV = 16;
  GetRhsVar(17, "d", &mV,      &nV,      &pV);               V  = 17;
  GetRhsVar(18, "i", &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 18;
  GetRhsVar(19, "i", &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 19;
  GetRhsVar(20, "d", &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 20;
  GetRhsVar(21, "d", &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 21;
  GetRhsVar(22, "i", &mINFO,   &nINFO,   &pINFO);          INFO = 22;
  
  LWORKL = mWORKL*nWORKL;   LDV=MAX(1,*istk(pN)); LDZ=LDV;

  C2F(dneupd)(istk(pRVEC), cstk(pHOWMANY),  istk(pSELECT),
	      stk(pDr), stk(pDi), stk(pZ),   &LDZ,
	      stk(pSIGMAr), stk(pSIGMAi), stk(pWORKev),
              cstk(pBMAT), istk(pN), cstk(pWHICH),
              istk(pNEV), stk(pTOL), stk(pRESID), 
              istk(pNCV), stk(pV), &LDV, 
	      istk(pIPARAM), istk(pIPNTR),
               stk(pWORKD), stk(pWORKL), &LWORKL, 
              istk(pINFO), 1L, 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("dneupd", istk(pINFO), 6L);
    return 0;
  }
  LhsVar(1)=Dr;    LhsVar(2)=Di;  LhsVar(3)=Z; 
  LhsVar(4)=RESID; LhsVar(5)=V; LhsVar(6)=IPARAM; 
  LhsVar(7)=IPNTR;  LhsVar(8)=WORKD; LhsVar(9)=WORKL; LhsVar(10)=INFO;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
