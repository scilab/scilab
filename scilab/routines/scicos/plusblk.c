/* Copyright INRIA */
#include "../machine.h"

/* Element wise som */

void 
plusblk(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	       ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     integer *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[];
     integer *nin,outsz[],*nout;
     double x[],xd[],z[],tvec[],rpar[];
     double *inptr[],*outptr[],*t;
{
    int k,i,n;
    double *y;
    double *u;

    n=outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

    y=(double *)outptr[0];

    for (i=0;i<n;i++) {
      y[i]=0.0;
      for (k=0;k<*nin;k++) {
	u=(double *)inptr[k];
	y[i]=y[i]+u[i];
      }
    }
}
