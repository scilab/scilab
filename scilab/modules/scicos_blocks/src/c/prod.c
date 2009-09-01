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
/*--------------------------------------------------------------------------*/ 
#include "dynlib_scicos_blocks.h"

SCICOS_BLOCKS_IMPEXP void prod(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
		  ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
		  
int *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar,insz[];
int *nin,outsz[],*nout;
double x[],xd[],z[],tvec[],rpar[];
double *inptr[],*outptr[],*t;

{
	int n = outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */
	double *y = (double *)outptr[0];
	int i = 0;

	for (i=0;i<n;i++) 
	{
		int k = 0;

		y[i] = 1.0;

		for (k=0;k<*nin;k++) 
		{
			double *u = (double *)inptr[k];
			y[i] = y[i]*u[i];
		}
	}
}
/*--------------------------------------------------------------------------*/ 
