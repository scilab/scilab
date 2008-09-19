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

void gainblk_ui16n(scicos_block *block,int flag)
{
 if ((flag==1)|(flag==6)){
  int i,j,l,ji,jl,il;
  unsigned short *u,*y;
  int mu,ny,my,mo,no;
  unsigned short *opar;
  double k,D,C,t;

  mo=GetOparSize(block,1,1);
  no=GetOparSize(block,1,2);
  mu=GetInPortRows(block,1);
  my=GetOutPortRows(block,1);
  ny=GetOutPortCols(block,1);
  u=Getuint16InPortPtrs(block,1);
  y=Getuint16OutPortPtrs(block,1);
  opar=Getuint16OparPtrs(block,1);

  k=pow(2,16);
  if (mo*no==1){
    for (i=0;i<ny*mu;++i){
     D=(double)(opar[0])*(double)(u[i]);
     t=D-(double)((int)(D/(k)))*((k));
     if ((t>=k/2)|(-(t)>=k/2))
       {if (t>=0) (t)=(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));
	else (t)=-(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));}
     y[i]=(unsigned short)t;
    }
  }else{
     for (l=0;l<ny;l++)
	 {for (j=0;j<my;j++)
	      {D=0;
	       for (i=0;i<mu;i++)
		   {ji=j+i*my;
		    jl=j+l*my;
		    il=i+l*mu;
		    C=(double)(opar[ji])*(double)(u[il]);
		    D=D + C;}
		    t=D-(double)((int)(D/(k)))*((k));
		    if ((t>=k/2)|(-(t)>=k/2))
		       {if (t>=0) (t)=(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));
			else (t)=-(-((k/2))+fabs(t-(double)((int)((t)/((k/2))))*((k/2))));}
		    y[jl]=(unsigned short)t;
		  }
	     }
  }
 }
}
