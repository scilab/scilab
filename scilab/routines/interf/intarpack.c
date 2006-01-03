#include <math.h>
#include <string.h>

#include "../stack-c.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#if WIN32
extern char *GetExceptionString(DWORD ExceptionCode);
#endif


#define MAX(x,y)	(((x)>(y))?(x):(y))

#define CHAR(x)         (cstk(x))
#define INT(x)  	(istk(x))
#define DOUBLE(x)	( stk(x))
#define CMPLX(x)	(zstk(x))

typedef int (*GatefuncS) __PARAMS((char *fname, int l)); 
typedef int mxArray;
typedef int (*GatefuncH) __PARAMS((int nlhs,mxArray *plhs[],int nrhs, mxArray *prhs[]));
typedef int (*Myinterfun) __PARAMS((char *, GatefuncH F));
typedef int (*GT) ();

typedef struct table_struct {
  Myinterfun f;    /** interface **/
  GT F;     /** function **/
  char *name;      /** its name **/
} GenericTable;


extern int sci_gateway __PARAMS((char *fname, GatefuncS F));

extern int  C2F(dsaupd) __PARAMS((int *ido, char *bmat, int *n, char *which, int *nev, double *tol, double *resid, int *ncv, double *v, int *ldv, int *iparam, int *ipntr, double *workd, double *workl, int *lworkl, int *info, unsigned long bmat_len, unsigned long which_len));

extern int  C2F(dnaupd) __PARAMS((int *ido, char *bmat, int *n, char *which, int *nev, double *tol, double *resid, int *ncv, double *v, int *ldv, int *iparam, int *ipntr, double *workd, double *workl, int *lworkl, int *info, unsigned long bmat_len, unsigned long which_len));

extern int C2F(dseupd) __PARAMS((int *rvec, char *howmny, int *select, double *d, double *z, int *ldz, double *sigma, char *bmat, int *n, char *which, int *nev ,double *tol, double *resid, int *ncv, double *v ,int *ldv, int *iparam,int *ipntr, double *workd,double *workl,int *lworkl,int *info, unsigned long rvec_length, unsigned long howmany_length, unsigned long bmat_length, unsigned long which_len));

extern int C2F(dneupd) __PARAMS((int *rvec, char *howmny, int *select, double *dr, double *di, double *z, int *ldz, double *sigmar, double *sigmai, double *workev, char *bmat, int *n, char *which, int *nev, double *tol, double *resid, int *ncv, double *v, int *ldv, int *iparam, int *ipntr, double *workd, double *workl, int *lworkl, int *info, unsigned long howmany_length, unsigned long bmat_length, unsigned long which_length));

#if WIN32
extern int C2F(znaupd)();
extern int C2F(zneupd)();
#endif
int intdsaupd(fname)
     char* fname;
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

  /* [IDO,RESID,V,IPARAM,IPNTR,WORKD,WORKL,INFO]=dsaupd...
     (ID0,BMAT,N,WHICH,NEV,TOL,RESID,NCV,V,IPARAM,IPNTR,WORKD,WORKL,INFO) */

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);
  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1, "i", &mIDO,    &nIDO,    &pIDO);           IDO  =  1;
  GetRhsVar( 2, "c", &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  2;
  GetRhsVar( 3, "i", &mN,      &nN,      &pN);             N    =  3;  
  GetRhsVar( 4, "c", &mWHICH,  &nWHICH,  &pWHICH);       WHICH  =  4; 
  GetRhsVar( 5, "i", &mNEV,    &nNEV,    &pNEV);            NEV =  5;
  GetRhsVar( 6, "d", &mTOL,    &nTOL,    &pTOL);            TOL =  6; 
  GetRhsVar( 7, "d", &mRESID,  &nRESID,  &pRESID);        RESID =  7;
  GetRhsVar( 8, "i", &mNCV,    &nNCV,    &pNCV);            NCV =  8;
  GetRhsVar( 9, "d", &mV,      &nV,      &pV);               V  =  9;
  GetRhsVar(10, "i", &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 10;
  GetRhsVar(11, "i", &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 11;
  GetRhsVar(12, "d", &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 12;
  GetRhsVar(13, "d", &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 13;
  GetRhsVar(14, "i", &mINFO,   &nINFO,   &pINFO);          INFO = 14;

  LWORKL = mWORKL*nWORKL;   LDV=MAX(1, *istk(pN));

  C2F(dsaupd)(INT(pIDO), CHAR(pBMAT), INT(pN),
	      CHAR(pWHICH), INT(pNEV), DOUBLE(pTOL), 
              DOUBLE(pRESID), INT(pNCV), DOUBLE(pV), &LDV, 
              INT(pIPARAM), INT(pIPNTR), DOUBLE(pWORKD), 
              DOUBLE(pWORKL), &LWORKL, INT(pINFO), 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("dsaupd", istk(pINFO), 6L);
    return 0;
  }
  LhsVar(1)=IDO;    LhsVar(2)=RESID; LhsVar(3)=V; 
  LhsVar(4)=IPARAM; LhsVar(5)=IPNTR; 
  LhsVar(6)=WORKD;  LhsVar(7)=WORKL; LhsVar(8)=INFO;
  return 0;
}

int intdnaupd(fname)
     char* fname;
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

  /* [IDO,RESID,V,IPARAM,IPNTR,WORKD,WORKL,INFO]=dnaupd...
     (ID0,BMAT,N,WHICH,NEV,TOL,RESID,NCV,V,IPARAM,IPNTR,WORKD,WORKL,INFO) */

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);
  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1, "i", &mIDO,    &nIDO,    &pIDO);           IDO  =  1;
  GetRhsVar( 2, "c", &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  2;
  GetRhsVar( 3, "i", &mN,      &nN,      &pN);             N    =  3;  
  GetRhsVar( 4, "c", &mWHICH,  &nWHICH,  &pWHICH);       WHICH  =  4; 
  GetRhsVar( 5, "i", &mNEV,    &nNEV,    &pNEV);            NEV =  5;
  GetRhsVar( 6, "d", &mTOL,    &nTOL,    &pTOL);            TOL =  6; 
  GetRhsVar( 7, "d", &mRESID,  &nRESID,  &pRESID);        RESID =  7;
  GetRhsVar( 8, "i", &mNCV,    &nNCV,    &pNCV);            NCV =  8;
  GetRhsVar( 9, "d", &mV,      &nV,      &pV);               V  =  9;
  GetRhsVar(10, "i", &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 10;
  GetRhsVar(11, "i", &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 11;
  GetRhsVar(12, "d", &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 12;
  GetRhsVar(13, "d", &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 13;
  GetRhsVar(14, "i", &mINFO,   &nINFO,   &pINFO);          INFO = 14;

  LWORKL = mWORKL*nWORKL;   LDV=MAX(1,*istk(pN));

  C2F(dnaupd)(istk(pIDO),   cstk(pBMAT),  istk(pN),
	      cstk(pWHICH), istk(pNEV),   stk(pTOL), 
               stk(pRESID), istk(pNCV),   stk(pV), &LDV, 
              istk(pIPARAM),istk(pIPNTR), stk(pWORKD), 
               stk(pWORKL), &LWORKL,      istk(pINFO), 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("dnaupd", istk(pINFO), 6L);
    return 0;
  }
  LhsVar(1)=IDO;    LhsVar(2)=RESID; LhsVar(3)=V; 
  LhsVar(4)=IPARAM; LhsVar(5)=IPNTR; 
  LhsVar(6)=WORKD;  LhsVar(7)=WORKL; LhsVar(8)=INFO;
  return 0;
}

int intznaupd(fname)
     char* fname;
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

int intdseupd(fname)
     char* fname;
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
  GetRhsVar( 1, "i", &mRVEC,   &nRVEC,   &pRVEC);         RVEC  =  1;
  GetRhsVar( 2, "c", &mHOWMANY,&nHOWMANY,&pHOWMANY);   HOWMANY  =  2;
  GetRhsVar( 3, "i", &mSELECT, &nSELECT, &pSELECT);     SELECT  =  3;
  GetRhsVar( 4, "d", &mD,      &nD,      &pD);               D  =  4;
  GetRhsVar( 5, "d", &mZ,      &nZ,      &pZ);               Z  =  5;
  GetRhsVar( 6, "d", &mSIGMA,  &nSIGMA,  &pSIGMA);        SIGMA =  6;
  GetRhsVar( 7, "c", &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  7;
  GetRhsVar( 8, "i", &mN,      &nN,      &pN);             N    =  8;  
  GetRhsVar( 9, "c", &mWHICH,  &nWHICH,  &pWHICH);       WHICH  =  9; 
  GetRhsVar(10, "i", &mNEV,    &nNEV,    &pNEV);            NEV = 10;
  GetRhsVar(11, "d", &mTOL,    &nTOL,    &pTOL);            TOL = 11; 
  GetRhsVar(12, "d", &mRESID,  &nRESID,  &pRESID);        RESID = 12;
  GetRhsVar(13, "i", &mNCV,    &nNCV,    &pNCV);            NCV = 13;
  GetRhsVar(14, "d", &mV,      &nV,      &pV);               V  = 14;
  GetRhsVar(15, "i", &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 15;
  GetRhsVar(16, "i", &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 16;
  GetRhsVar(17, "d", &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 17;
  GetRhsVar(18, "d", &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 18;
  GetRhsVar(19, "i", &mINFO,   &nINFO,   &pINFO);          INFO = 19;

  LWORKL = mWORKL*nWORKL;   LDV=MAX(1,*istk(pN)); LDZ=LDV;

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
  return 0;
}

int intdneupd(fname)
     char* fname;
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

int intzneupd(fname)
     char* fname;
{ 
  int RVEC,     mRVEC,     nRVEC,      pRVEC;
  int HOWMANY,  mHOWMANY,  nHOWMANY,   pHOWMANY;
  int SELECT,   mSELECT,   nSELECT,    pSELECT;
  int D,        mD,        nD,         pD;
  int Z,        mZ,        nZ,         pZ;
  int SIGMA,    mSIGMA,    nSIGMA,     pSIGMA;
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
  int RWORK,    mRWORK,    nRWORK,     pRWORK;
  int INFO,     mINFO,     nINFO,      pINFO;

  int minlhs=1, minrhs=21, maxlhs=9, maxrhs=21;
  int LDZ, LDV, LWORKL;

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);
  /*                                                  VARIABLE = NUMBER   */
  GetRhsVar( 1, "i", &mRVEC,   &nRVEC,   &pRVEC);         RVEC  =  1;
  GetRhsVar( 2, "c", &mHOWMANY,&nHOWMANY,&pHOWMANY);   HOWMANY  =  2;
  GetRhsVar( 3, "i", &mSELECT, &nSELECT, &pSELECT);     SELECT  =  3;
  GetRhsVar( 4, "z", &mD,      &nD,      &pD);               D  =  4;
  GetRhsVar( 5, "z", &mZ,      &nZ,      &pZ) ;              Z  =  5;
  GetRhsVar( 6, "z", &mSIGMA,  &nSIGMA,  &pSIGMA);        SIGMA =  6;
  GetRhsVar( 7, "z", &mWORKev, &nWORKev, &pWORKev);      WORKev =  7;
  GetRhsVar( 8, "c", &mBMAT,   &nBMAT,   &pBMAT);          BMAT =  8;
  GetRhsVar( 9, "i", &mN,      &nN,      &pN);             N    =  9;  
  GetRhsVar(10, "c", &mWHICH,  &nWHICH,  &pWHICH);       WHICH  = 10; 
  GetRhsVar(11, "i", &mNEV,    &nNEV,    &pNEV);            NEV = 11;
  GetRhsVar(12, "d", &mTOL,    &nTOL,    &pTOL);            TOL = 12; 
  GetRhsVar(13, "z", &mRESID,  &nRESID,  &pRESID);        RESID = 13;
  GetRhsVar(14, "i", &mNCV,    &nNCV,    &pNCV);            NCV = 14;
  GetRhsVar(15, "z", &mV,      &nV,      &pV);               V  = 15;
  GetRhsVar(16, "i", &mIPARAM, &nIPARAM, &pIPARAM);      IPARAM = 16;
  GetRhsVar(17, "i", &mIPNTR,  &nIPNTR,  &pIPNTR);       IPNTR  = 17;
  GetRhsVar(18, "z", &mWORKD,  &nWORKD,  &pWORKD);       WORKD  = 18;
  GetRhsVar(19, "z", &mWORKL,  &nWORKL,  &pWORKL);       WORKL  = 19;
  GetRhsVar(20, "d", &mRWORK,  &nRWORK,  &pRWORK);       RWORK  = 20;
  GetRhsVar(21, "i", &mINFO,   &nINFO,   &pINFO);          INFO = 21;

  LWORKL = mWORKL*nWORKL;   LDV=MAX(1,*istk(pN)); LDZ=LDV;

  C2F(zneupd)(istk(pRVEC), cstk(pHOWMANY), istk(pSELECT), zstk(pD), 
              zstk(pZ), &LDZ, zstk(pSIGMA), zstk(pWORKev),
              cstk(pBMAT), istk(pN), cstk(pWHICH), istk(pNEV), 
              stk(pTOL), zstk(pRESID), istk(pNCV), zstk(pV), 
              &LDV, istk(pIPARAM), istk(pIPNTR), zstk(pWORKD), 
              zstk(pWORKL), &LWORKL, stk(pRWORK), istk(pINFO), 
              1L, 1L, 2L);

  if (*istk(pINFO) < 0) {
    C2F(errorinfo)("zneupd", istk(pINFO), 6L);
    return 0;
  }
  LhsVar(1)=D;     LhsVar(2)=Z;  LhsVar(3)=RESID; 
  LhsVar(4)=IPARAM; LhsVar(5)=IPNTR;   
  LhsVar(6)=WORKD; LhsVar(7)=WORKL; 
  LhsVar(8)=RWORK; LhsVar(9)=INFO;
  return 0;
}

static GenericTable Tab[]={
  {(Myinterfun) sci_gateway, intdsaupd,"dsaupd"},
  {(Myinterfun) sci_gateway, intdnaupd,"dnaupd"},
  {(Myinterfun) sci_gateway, intznaupd,"znaupd"},
  {(Myinterfun) sci_gateway, intdseupd,"dseupd"},
  {(Myinterfun) sci_gateway, intdneupd,"dneupd"},
  {(Myinterfun) sci_gateway, intzneupd,"zneupd"},
};
 
int C2F(intarpack)()
{
	Rhs = Max(0, Rhs);
	
	#if WIN32
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
		#endif
	#else
		(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
	#endif

	return 0;
}


