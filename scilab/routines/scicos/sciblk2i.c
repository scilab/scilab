/* Copyright INRIA */
#include "../machine.h"

extern integer C2F(scierr)();
extern void C2F(itosci)();
extern void C2F(dtosci)();
extern void C2F(vvtosci)();
extern void C2F(scitovv)();
extern void C2F(skipvars)();
extern void C2F(scitod)();
extern void C2F(list2vars)();
extern void C2F(ltopadj)();
extern void C2F(scifunc)();
extern int C2F(mklist)();


void 
sciblk2i(flag,nevprt,t,residual,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)

integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[],*nin,outsz[],*nout;
double residual[],x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;

{
    int k;
    double *y;
    double *u;
/*    int nev,ic;*/
    integer one=1,skip;
    integer nu,ny;
    integer mlhs=6,mrhs=9;
    integer ltop;
    /*
 
    [y,  x,  z,  tvec,xd]=func(flag,nevprt,t,xd,x,z,rpar,ipar,u)
    [y,  x,  z,  tvec,res]=func(flag,nevprt,t,xd,x,z,rpar,ipar,u)
    */
    C2F(itosci)(flag,&one,&one);
    if (C2F(scierr)()!=0) goto err;
    C2F(itosci)(nevprt,&one,&one);
    if (C2F(scierr)()!=0) goto err;  
    C2F(dtosci)(t,&one,&one);
    if (C2F(scierr)()!=0) goto err;  
    C2F(dtosci)(xd,nx,&one);
    if (C2F(scierr)()!=0) goto err;  
    C2F(dtosci)(x,nx,&one);
    if (C2F(scierr)()!=0) goto err;  
    C2F(vvtosci)(z,nz);
    if (C2F(scierr)()!=0) goto err; 
    C2F(vvtosci)(rpar,nrpar); 
    if (C2F(scierr)()!=0) goto err;  
    C2F(itosci)(ipar,nipar,&one);
    if (C2F(scierr)()!=0) goto err;
    for (k=0;k<*nin;k++) {
	u=(double *)inptr[k];
	nu=insz[k];
	C2F(dtosci)(u,&nu,&one);
	if (C2F(scierr)()!=0) goto err;
    }
    C2F(mklist)(nin);


    C2F(scifunc)(&mlhs,&mrhs);
    if (C2F(scierr)()!=0) goto err;
    
    switch (*flag) {
    case 1 :
        /* y  computation */
      {
	skip=4;
	C2F(skipvars)(&skip);
      }
      if (*nout==0) {
	skip=1;
	C2F(skipvars)(&skip);
      }
      else {
	C2F(list2vars)(nout,&ltop);
	if (C2F(scierr)()!=0) goto err; 
	for (k=*nout-1;k>=0;k--) {
	  y=(double *)outptr[k];
	  ny=outsz[k];
	  C2F(scitod)(y,&ny,&one);
	  if (C2F(scierr)()!=0) goto err;
	}
	/* list2vars has changed the Lstk(top+1) value. 
	   reset the correct value */
	C2F(ltopadj)(&ltop);  
      }
      break;
    case 0 :
	/*  residual  computation */
      {
	C2F(scitod)(residual,nx,&one);
	skip=4;
	C2F(skipvars)(&skip);
	break;
      }
    case 2 : 
      /* continuous and discrete state jump */
      {
	C2F(scitod)(xd,nx,&one);
	skip=1;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	C2F(scitod)(x,nx,&one);
	skip=1;
	C2F(skipvars)(&skip);
      }
      break;
    case 3 :
      /* output event */
	skip=1;
	C2F(skipvars)(&skip);
	C2F(scitod)(tvec,ntvec,&one);
        skip=3;
	C2F(skipvars)(&skip);
	break;
    case 4 :
	C2F(scitod)(xd,nx,&one);
	skip=1;C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	C2F(scitod)(x,nx,&one);
	skip=1;C2F(skipvars)(&skip);
	break;
    case 5 :
        C2F(scitod)(xd,nx,&one);
	skip=1;C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	C2F(scitod)(x,nx,&one);
        skip=1;
	C2F(skipvars)(&skip);
	break;
    case 6 :
        C2F(scitod)(xd,nx,&one);
	skip=1;
	C2F(skipvars)(&skip);
	C2F(scitovv)(z,nz);
	C2F(scitod)(x,nx,&one);
	if (*nout==0) {
	    skip=1;
	    C2F(skipvars)(&skip);
	}
	else {
	    C2F(list2vars)(nout,&ltop); 
	    if (C2F(scierr)()!=0) goto err;
	    for (k=*nout-1;k>=0;k--) {
		y=(double *)outptr[k];
		ny=outsz[k];
		C2F(scitod)(y,&ny,&one);
		if (C2F(scierr)()!=0) goto err;
	    }
	    /* list2vars has changed the Lstk(top+1) value. 
	       reset the correct value */
	   C2F(ltopadj)(&ltop);  
	}
	break;
    }
    return;
 err: 
    *flag=-1;
}
