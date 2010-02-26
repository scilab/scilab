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
#include <math.h>
#include "scicos.h"
#include "scicos_block4.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
#define matbscal_n(y1,u1,u2,mu,nu)  {for (i=0;i<mu*nu;i++) y1[i]=u1[i]*u2[0];}
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP void matbyscal(scicos_block *block,int flag)
{
 if (flag==1){
  int i = 0;

  int ut = GetInType(block,1);
  int mu = GetOutPortRows(block,1);
  int nu = GetOutPortCols(block,1);

  switch (ut)
  {
      case SCSREAL_N :{
           double *u1=GetRealInPortPtrs(block,1);
           double *u2=GetRealInPortPtrs(block,2);
           double *y1=GetRealOutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSINT32_N :{
           long *u1=Getint32InPortPtrs(block,1);
           long *u2=Getint32InPortPtrs(block,2);
           long *y1=Getint32OutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSINT16_N :{
           short *u1=Getint16InPortPtrs(block,1);
           short *u2=Getint16InPortPtrs(block,2);
           short *y1=Getint16OutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSINT8_N :{
           char *u1=Getint8InPortPtrs(block,1);
           char *u2=Getint8InPortPtrs(block,2);
           char *y1=Getint8OutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSUINT32_N :{
           unsigned long *u1=Getuint32InPortPtrs(block,1);
           unsigned long *u2=Getuint32InPortPtrs(block,2);
           unsigned long *y1=Getuint32OutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSUINT16_N :{
           unsigned short *u1=Getuint16InPortPtrs(block,1);
           unsigned short *u2=Getuint16InPortPtrs(block,2);
           unsigned short *y1=Getuint16OutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSUINT8_N :{
           unsigned char *u1=Getuint8InPortPtrs(block,1);
           unsigned char *u2=Getuint8InPortPtrs(block,2);
           unsigned char *y1=Getuint8OutPortPtrs(block,1);
           matbscal_n(y1,u1,u2,mu,nu);
           break;}

      case SCSCOMPLEX_N :{
           double *u1r=GetRealInPortPtrs(block,1);
           double *u2r=GetRealInPortPtrs(block,2);
           double *y1r=GetRealOutPortPtrs(block,1);
           double *u1i=GetImagInPortPtrs(block,1);
           double *u2i=GetImagInPortPtrs(block,2);
           double *y1i=GetImagOutPortPtrs(block,1);
           for (i=0;i<mu*nu;i++) 
	     {y1r[i]=(u1r[i]*u2r[0])-(u1i[i]*u2i[0]);
	       y1i[i]=(u1r[i]*u2i[0])+(u1i[i]*u2r[0]);}
	   break;}

      default :{
          set_block_error(-4); 
	  return;} 
   } 
  }

}
/*--------------------------------------------------------------------------*/ 
