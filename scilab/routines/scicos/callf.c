/* Copyright INRIA */
#include <string.h>
#include "../machine.h"
#include "../sun/link.h"
#include "scicos.h"

#include "blocks.h"


/* access to Fortran common at C level sciblks */

extern void  F2C(sciblk)();
extern void  sciblk2();
extern void  GetDynFunc();
extern void  sciprint();
extern void  C2F(iislink)();

void 
C2F(callf)(kfun,nclock,funptr,funtyp,t,xd,x,xptr,z,zptr,iz,izptr,
	   rpar,rpptr,ipar,ipptr,tvec,ntvec,inpptr,inplnk,outptr,
	   outlnk,lnkptr,outtb,flag) 
     integer *kfun,*nclock,*funptr,*funtyp,*xptr,*zptr,*iz,*izptr;
     integer *rpptr,*ipar,*ipptr;
     integer *ntvec,*inpptr,*inplnk,*outptr,*outlnk,*lnkptr,*flag;
     double *t,*xd,*x,*z,*rpar,*outtb,*tvec;
{
  voidf loc ;  
  double* args[SZ_SIZE];
  integer sz[SZ_SIZE];
  double intabl[TB_SIZE],outabl[TB_SIZE];
  int ii,i,kf,nx,nz,nrpar,nipar,in,out,ki,ko,ni,no;
  int nin,nout,lprt,szi,funtype;
  ScicosF0 loc0;
  ScicosF loc1;
  ScicosFm1 loc3;
  ScicosF2 loc2;

  kf=*kfun-1;
  i=funptr[kf];
  funtype=funtyp[kf];
  
  if (i<0) {
    switch (funtype) {
    case -1:
      sciprint("type -1 function not allowed for scilab blocks\r\n");
      *flag=-1000-(*kfun);
    case 0:
      loc=F2C(sciblk);
      break;
    case 1:
      sciprint("type 1 function not allowed for scilab blocks\r\n");
      *flag=-1000-(*kfun);
      return;
    case 2:
      sciprint("type 2 function not allowed for scilab blocks\r\n");
      *flag=-1000-(*kfun);
      break;
    case 3:
      loc=sciblk2;
      funtype=2;
      break;
    default :
      sciprint("Undefined Function type\r\n");
      *flag=-1000;
      return;
    }
    C2F(scsptr).ptr=-i; /* set scilab function adress for sciblk */
  }
  else if (i<=ntabsim)
    loc=*(tabsim[i-1].fonc);
  else {
    i -= (ntabsim+1);
    GetDynFunc(i,&loc);
    if ( loc == (voidf) 0)
      {
	sciprint("Function not found\r\n");
	*flag=-1000-(*kfun);
	return;
      }
  }
  
  nx=xptr[kf+1]-xptr[kf];
  nz=zptr[kf+1]-zptr[kf];
  nrpar=rpptr[kf+1]-rpptr[kf];
  nipar=ipptr[kf+1]-ipptr[kf];
  nin=inpptr[kf+1]-inpptr[kf]; /* number of input ports */
  nout=outptr[kf+1]-outptr[kf];/* number of output ports */
  switch (funtype) {
  case -1 :
    /* special synchro blocks */
    in=0;
    lprt=inplnk[inpptr[kf]-1+in];
    args[in]=&(outtb[lnkptr[lprt-1]-1]);
    sz[in]=lnkptr[lprt]-lnkptr[lprt-1];
    loc3 = (ScicosFm1) loc;
    (*loc3)(flag,nclock,ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,(double *)args[0],&sz[0]);
    break;   
  case 1 :			
    /* one entry for each input or output */
    for (in = 0 ; in < nin ; in++) 
      {
	lprt=inplnk[inpptr[kf]-1+in];
	args[in]=&(outtb[lnkptr[lprt-1]-1]);
	sz[in]=lnkptr[lprt]-lnkptr[lprt-1];
      }
    for (out=0;out<nout;out++) {
      lprt=outlnk[outptr[kf]-1+out];
      args[in+out]=&(outtb[lnkptr[lprt-1]-1]);
      sz[in+out]=lnkptr[lprt]-lnkptr[lprt-1];
    }
    loc1 = (ScicosF) loc;
    (*loc1)(flag,nclock,t,&(xd[xptr[kf]-1]),&(x[xptr[kf]-1]),&nx,&(z[zptr[kf]-1]),&nz,
	    tvec,ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,
	    (double *)args[0],&sz[0],
	    (double *)args[1],&sz[1],(double *)args[2],&sz[2],
	    (double *)args[3],&sz[3],(double *)args[4],&sz[4],
	    (double *)args[5],&sz[5],(double *)args[6],&sz[6],
	    (double *)args[7],&sz[7],(double *)args[8],&sz[8],
	    (double *)args[9],&sz[9],(double *)args[10],&sz[10],
	    (double *)args[11],&sz[11],(double *)args[12],&sz[12],
	    (double *)args[13],&sz[13],(double *)args[14],&sz[14],
	    (double *)args[15],&sz[15],(double *)args[16],&sz[16]); 
    break;   
  case 0 :			
    /* concatenated entries and concatened outputs */
    ni=0;
    /* catenate inputs if necessary */

    if (nin>1) {
      ki=0;
      for (in=0;in<nin;in++) {
	lprt=inplnk[inpptr[kf]-1+in];
	szi=lnkptr[lprt]-lnkptr[lprt-1];
	for (ii=0;ii<szi;ii++) 
	  intabl[ki++]=outtb[lnkptr[lprt-1]-1+ii];
	ni=ni+szi;
      }
      args[0]=&(intabl[0]);
    }
    else {
      if (nin==0) {
	ni=0;
	args[0]=&(outtb[0]);
      }
      else {
	lprt=inplnk[inpptr[kf]-1];
	args[0]=&(outtb[lnkptr[lprt-1]-1]);
	ni=lnkptr[lprt]-lnkptr[lprt-1];
      }
    }
    in=nin;
    
    /* catenate outputs if necessary */
    if (nout>1) {
      ko=0;
      for (out=0;out<nout;out++) {
	lprt=outlnk[outptr[kf]-1+out];
	szi=lnkptr[lprt]-lnkptr[lprt-1];
	
	for (ii=0;ii<szi;ii++)  
	  outabl[ko++]=outtb[lnkptr[lprt-1]-1+ii];
	no=no+szi;
      }
      args[1]=&(outabl[0]);
    }
    else {
      if (nout==0) {
	no=0;
	args[1]=&(outtb[0]);
      }
      else {
	lprt=outlnk[outptr[kf]-1];
	args[1]=&(outtb[lnkptr[lprt-1]-1]);
	no=lnkptr[lprt]-lnkptr[lprt-1];
      }
    }

    loc0 = (ScicosF0) loc;
    (*loc0)(flag,nclock,t,&(xd[xptr[kf]-1]),&(x[xptr[kf]-1]),&nx,&(z[zptr[kf]-1]),&nz,
	    tvec,ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,(double *)args[0],&ni,
	    (double *)args[1],&no);

    /* split output vector on each port if necessary */
    if (nout>1) {
      ko=0;
      for (out=0;out<nout;out++) {
	lprt=outlnk[outptr[kf]-1+out];
	szi=lnkptr[lprt]-lnkptr[lprt-1];
	for (ii=0;ii<szi;ii++)  
	  outtb[lnkptr[lprt-1]-1+ii]=outabl[ko++];
      }
    }
    break;
  case 2 :			
    /* inputs and outputs given by a table of pointers */
    for (in=0;in<nin;in++) {
      lprt=inplnk[inpptr[kf]-1+in];
      args[in]=&(outtb[lnkptr[lprt-1]-1]);
      sz[in]=lnkptr[lprt]-lnkptr[lprt-1];
    }
    for (out=0;out<nout;out++) {
      lprt=outlnk[outptr[kf]-1+out];
      args[in+out]=&(outtb[lnkptr[lprt-1]-1]);
      sz[in+out]=lnkptr[lprt]-lnkptr[lprt-1];
    }
    loc2 = (ScicosF2) loc;
    (*loc2)(flag,nclock,t,&(xd[xptr[kf]-1]),&(x[xptr[kf]-1]),&nx,
	    &(z[zptr[kf]-1]),&nz,
	    tvec,ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,&(args[0]),&(sz[0]),&nin,
	    &(args[in]),&(sz[in]),&nout);
    break;
  default:
    sciprint("Undefined Function type\r\n");
    *flag=-1000;
    return;
  }
}

/* ? */

integer C2F(funnum)(fname)
     char * fname;
{
  int i=0,ln;
  integer loc=-1;
  while ( tabsim[i].name != (char *) NULL) {
	if ( strcmp(fname,tabsim[i].name) == 0 ) return(i+1);
	i++;
    }
  ln=strlen(fname);
  C2F(iislink)(fname,&loc);
  if (loc >= 0) return(ntabsim+(int)loc+1);
  return(0);
}

/* ? */

integer C2F(getscsmax)(max_sz,max_tb)
     integer *max_sz, *max_tb;
{
  *max_sz=SZ_SIZE;
  *max_tb=TB_SIZE;
}
