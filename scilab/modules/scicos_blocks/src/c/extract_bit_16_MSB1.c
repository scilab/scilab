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
#include "scicos_block4.h"
#include <math.h>

void extract_bit_16_MSB1(scicos_block *block,int flag)
{
   int i,maxim;
   short *y,*u,ref,n;
   int *ipar;
   y=Getint16OutPortPtrs(block,1);
   u=Getint16InPortPtrs(block,1);
   ipar=GetIparPtrs(block);
   maxim=16;
   ref=0;
   for(i=0;i<*ipar;i++)
       {n=(short)pow(2,maxim-1-i);
        ref=ref+n;}
   *y=(*u)&(ref);
   *y=*y>>(maxim-*ipar);
}
