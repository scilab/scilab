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
#include <math.h>
#include "scicos_block4.h"
#include <stdio.h>

void summation_i32n(scicos_block *block,int flag)
{
 if((flag==1)|(flag==6)) {
    int j,k;
    int nu,mu,nin;
    long *y;
    int *ipar;
    double *rpar;
    long *u;

    y=Getint32OutPortPtrs(block,1);
    nu=GetInPortRows(block,1);
    mu=GetInPortCols(block,1);
    ipar=GetIparPtrs(block);
    rpar=GetRparPtrs(block);
    nin=GetNin(block);

    if (nin==1){
      y[0]=0;
      u=Getint32InPortPtrs(block,1);
      for (j=0;j<nu*mu;j++) {
	y[0]=y[0]+u[j];
      }
    }
    else {
      for (j=0;j<nu*mu;j++) {
        y[j]=0;
	for (k=0;k<nin;k++) {
	  u=Getint32InPortPtrs(block,k+1);
          if(ipar[k]>0){
	     y[j]=y[j]+u[j];
	    }
	  else{
	     y[j]=y[j]-u[j];}
      }
    }
   }
  }
}
