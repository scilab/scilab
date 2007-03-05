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
extern int C2F(znaupd)();
/*-----------------------------------------------------------------------------------*/
int C2F(intznaupd) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(intznaupd) _PARAMS((char *fname,unsigned long fname_len))
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
  GetRhsVar( 1, "i", &mIDO,    &nIDO,    &pIDO);           IDO  =  1;
  GetRhsVar( 2, "c", &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  2;
  GetRhsVar( 3, "i", &mN,      &nN,      &pN);             N    =  3;  
  GetRhsVar( 4, "c", &mWHICH,  &nWHICH,  &pWHICH);       WHICH  =  4; 
  GetRhsVar( 5, "i", &mNEV,    &nNEV,    &pNEV);            NEV =  5;
  GetRhsVar( 6, "d", &mTOL,    &nTOL,    &pTOL);            TOL =  6; 
  GetRhsVar( 7, "z", &mRESID,  &nRESID,  &pRESID);        RESID =  7;
  GetRhsVar( 8, "i", &mNCV,    &nNCV,    &pNCV);            NCV =  8;
  GetRhsVar( 9, "z", &mV,      &nV,      &pV);               V  =  9;
  GetRhsVar(10, "i", &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 10;
  GetRhsVar(11, "i", &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 11;
  GetRhsVar(12, "z", &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 12;
  GetRhsVar(13, "z", &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 13;
  GetRhsVar(14, "d", &mRWORK,  &nRWORK,  &pRWORK);       RWORK  = 14;
  GetRhsVar(15, "i", &mINFO,   &nINFO,   &pINFO);          INFO = 15;

  LWORKL = mWORKL*nWORKL;   LDV=MAX(1,*istk(pN));

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
  return 0;
}
/*-----------------------------------------------------------------------------------*/
