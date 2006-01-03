#include <math.h>
#include <string.h>

#include "../stack-c.h"

#if WIN32
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif

#define MAX(x,y)	(((x)>(y))?(x):(y))
#define MIN(x,y)	(((x)<(y))?(x):(y))

typedef int mxArray;
typedef int (*GatefuncH) __PARAMS((int nlhs,mxArray *plhs[],int nrhs, mxArray *prhs[]));
typedef int (*GatefuncS) __PARAMS((char *fname, int l)); 
typedef int Gatefunc __PARAMS((int nlhs,mxArray *plhs[],int nrhs,
                                 mxArray *prhs[]));
typedef int (*FGatefuncH) __PARAMS((int *nlhs,mxArray *plhs[],int *nrhs,
                                 mxArray *prhs[]));

typedef int (*Myinterfun) __PARAMS((char *, GatefuncH F));
typedef int (*GT) ();

typedef struct table_struct {
  Myinterfun f;    /** interface **/
  GT F;     /** function **/
  char *name;      /** its name **/
} GenericTable;

extern int fortran_mex_gateway __PARAMS((char *fname, FGatefuncH F));
extern int sci_gateway __PARAMS((char *fname, GatefuncS F));

extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));

extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);
extern Gatefunc C2F(intmucomp);
extern Gatefunc C2F(intricc2);
extern Gatefunc C2F(inthinf);
extern Gatefunc C2F(intdhinf);
extern Gatefunc C2F(intlinmeq);
extern int C2F(intmb03od) __PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intzb03od) __PARAMS((char *fname, unsigned long fname_len));
#if WIN32
extern int C2F(ab01od)();
#endif

int intrankqr(fname)
     char* fname;
{
  int *header1;
  int Cmplx;int ret;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];
  if (Cmplx==0) {
    ret = C2F(intmb03od)("rankqr",6L);
    return 0; } 
  else
    {
      ret = C2F(intzb03od)("rankqr",6L);
      return 0; } 
}

int intab01od(fname)
     char* fname;
{ 
  int mA,nA,ptrA, mB,nB, ptrB;
  int A,B,U,KSTAIR,V, ptrIWORK,ptrU,ptrTOL;
  int ptrKSTAIR,ptrV,ptrDWORK,ptrJUNK,ptrNCONT;
  int LDA, LDB, LDU, LDV, LDWORK;
  int N, M, mtol, ntol; int un; int INFO, INDCON, NCONT;
  char  *JOBU, *JOBV;
  double theTOL;
  int minlhs=1, minrhs=2, maxlhs=6, maxrhs=3;

  /*     [NCONT,U,KSTAIR,V,A,B]=ab01od(A,B,[TOL])   */

  CheckRhs(minrhs,maxrhs);  CheckLhs(minlhs,maxlhs);
  theTOL=(double) C2F(dlamch)("e",1L);
  GetRhsVar(1,"d",&mA,&nA,&ptrA);   A=1;        /*     A */
  N=mA;
  theTOL=0.2*sqrt(2*theTOL)*N;
  GetRhsVar(2,"d",&mB,&nB,&ptrB);   B=2;        /*     B */
  M=nB;
  if (nA != mB || mA != nA )
    { Scierror(999,"Invalid A,B matrices \r\n");  return 0; }
  if (Rhs == 3) {
    /*    TOL is given:   ab01od(A,B,tol)   */
    GetRhsVar(3,"d",&mtol,&ntol,&ptrTOL);  theTOL=*stk(ptrTOL);    /*     TOL */
    if (theTOL>1.0||theTOL<0.0) {
      Scierror(999,"TOL must be in [0 1]\r\n");  return 0; 
    }
  }

  /*     dimensions...    */
  LDA=MAX(1,N);  LDB=LDA;  LDU=LDA; LDV=MAX(1,M);
  LDWORK = MAX(1, N*M + MAX(N,M) + MAX(N,3*M));

  /*     other parameters of AB01OD   */
  JOBU= "N"; if (Lhs >= 2)  JOBU="I";
  JOBV= "N"; if (Lhs >= 4)  JOBV="I";

  /*     creating NCONT,U,KSTAIR,V,IWORK,DWORK   */
  CreateVar(Rhs+1,"i",(un=1,&un),(un=1,&un),&ptrNCONT);  NCONT=Rhs+1;
  CreateVar(Rhs+2,"d",&N,&N,&ptrU);  U=Rhs+2;
  CreateVar(Rhs+3,"i",(un=1,&un),&N,&ptrKSTAIR);  KSTAIR=Rhs+3;
  CreateVar(Rhs+4,"d",&M,&M,&ptrV);  V=Rhs+4;
  CreateVar(Rhs+5,"i",(un=1,&un),&M,&ptrIWORK);
  CreateVar(Rhs+6,"d",(un=1,&un),&LDWORK,&ptrDWORK);
  C2F(ab01od)( "A", JOBU, JOBV, &N, &M, stk(ptrA), &LDA, 
               stk(ptrB), &LDB, stk(ptrU), &LDU, stk(ptrV), &LDV, 
	       istk(ptrNCONT), &INDCON, istk(ptrKSTAIR), &theTOL,  
               istk(ptrIWORK), stk(ptrDWORK), &LDWORK, &INFO );
  if (INFO != 0) {
    C2F(errorinfo)("ab01od", &INFO, 6L);
    return 0;
  }
  if (Lhs >= 3) {
    /*     resizing KSTAIR      */
    CreateVar(Rhs+7,"i",(un=1,&un),&INDCON,&ptrJUNK); 
    KSTAIR=Rhs+7;
    C2F(icopy)(&INDCON,istk(ptrKSTAIR),(un=1,&un),istk(ptrJUNK),(un=1,&un)); }
  /*     lhs variables: [NCONT,U,KSTAIR,V,A,B]=ab01od(A,B)   */
  LhsVar(1)=NCONT; LhsVar(2)=U; 
  LhsVar(3)=KSTAIR; LhsVar(4)=V; 
  LhsVar(5)=A; LhsVar(6)=B;
  return 0;
}


static GenericTable Tab[]={
  {(Myinterfun) fortran_mex_gateway, C2F(sident),"sident"},
  {(Myinterfun) fortran_mex_gateway, C2F(sorder),"sorder"},
  {(Myinterfun) fortran_mex_gateway, C2F(findbd),"findbd"},
  {(Myinterfun) sci_gateway, intrankqr,"rankqr"},
  {(Myinterfun) sci_gateway, intab01od,"contr"},
  {(Myinterfun) sci_gateway, C2F(intmucomp),"mucomp"},
  {(Myinterfun) sci_gateway, C2F(intricc2),"pet_ricc"},
  {(Myinterfun) sci_gateway, C2F(inthinf),"hinf"},
  {(Myinterfun) sci_gateway, C2F(intdhinf),"dhinf"},
  {(Myinterfun) sci_gateway, C2F(intlinmeq),"linmeq"},
};
 
int C2F(intslicot)()
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


