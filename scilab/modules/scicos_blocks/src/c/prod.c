/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/* Copyright INRIA */
#include "machine.h"

/* Element wise product */

void 
prod(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
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
      y[i]=1.0;
      for (k=0;k<*nin;k++) {
	u=(double *)inptr[k];
	y[i]=y[i]*u[i];
      }
    }
}
