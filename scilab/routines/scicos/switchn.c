/* Copyright INRIA */
#include "../machine.h"

void 
switchn(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[],*nin,outsz[],*nout;
double x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;

{

    int k;
    double *y;
    double *u;
    int /*nev,*/ic;
    ic=ipar[0];
    if (*nin>1) {
      y=(double *)outptr[0];
      u=(double *)inptr[ic];
      for (k=0;k<outsz[0];k++)
	*(y++)=*(u++);  
    }
    else {
      y=(double *)outptr[ic];
      u=(double *)inptr[0];
      for (k=0;k<outsz[0];k++)
	*(y++)=*(u++);  
    }
}



